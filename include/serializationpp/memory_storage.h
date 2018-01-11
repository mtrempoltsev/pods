#pragma once

#include <cassert>
#include <limits>
#include <type_traits>
#include <vector>

#include "details/memory_managers.h"

namespace spp
{
    class ReadOnlyMemoryStorage final
    {
    public:
        ReadOnlyMemoryStorage(const char* data, size_t size) noexcept
            : maxSize_(size)
            , data_(data)
            , pos_(0)
        {
            assert(size > 0);
        }

        ReadOnlyMemoryStorage(const ReadOnlyMemoryStorage&) = delete;
        ReadOnlyMemoryStorage& operator=(const ReadOnlyMemoryStorage&) = delete;

        ReadOnlyMemoryStorage(ReadOnlyMemoryStorage&&) = delete;
        ReadOnlyMemoryStorage& operator=(ReadOnlyMemoryStorage&&) = delete;

        bool get(bool& value) noexcept
        {
            if (pos_ + sizeof(bool) > maxSize_)
                return false;
            value = data_[pos_] == 1;
            pos_ += sizeof(bool);
            return true;
        }

        template <class T>
        bool get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 1, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) > maxSize_)
                return false;
            value = data_[pos_];
            pos_ += sizeof(T);
            return true;
        }

        template <class T>
        bool get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 2, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) > maxSize_)
                return false;
            auto to = reinterpret_cast<char*>(&value);
            auto from = data_ + pos_;
            to[0] = from[0];
            to[1] = from[1];
            pos_ += sizeof(T);
            return true;
        }

        template <class T>
        bool get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 4, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) > maxSize_)
                return false;
            auto to = reinterpret_cast<char*>(&value);
            auto from = data_ + pos_;
            to[0] = from[0];
            to[1] = from[1];
            to[2] = from[2];
            to[3] = from[3];
            pos_ += sizeof(T);
            return true;
        }

        template <class T>
        bool get(T& value, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 8, int>::type = 0) noexcept
        {
            if (pos_ + sizeof(T) > maxSize_)
                return false;
            auto to = reinterpret_cast<char*>(&value);
            auto from = data_ + pos_;
            to[0] = from[0];
            to[1] = from[1];
            to[2] = from[2];
            to[3] = from[3];
            to[4] = from[4];
            to[5] = from[5];
            to[6] = from[6];
            to[7] = from[7];
            pos_ += sizeof(T);
            return true;
        }

        template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
        bool get(T& value) noexcept
        {
            std::underlying_type_t<T> tmp;
            if (!get(tmp))
                return false;
            value = static_cast<T>(tmp);
            return true;
        }

        bool get(char* data, size_t size) noexcept
        {
            if (pos_ + size > maxSize_)
                return false;
            auto begin = data_ + pos_;
            std::copy(begin, begin + size, data);
            pos_ += size;
            return true;
        }

    private:
        const size_t maxSize_;
        const char* const data_;
        size_t pos_;
    };

    namespace details
    {
        template <class MemoryManager>
        class WriteOnlyMemoryStorage
        {
        public:
            explicit WriteOnlyMemoryStorage(MemoryManager&& memoryManager) noexcept
                : memoryManager_(std::forward<MemoryManager>(memoryManager))
            {
            }

            WriteOnlyMemoryStorage(const WriteOnlyMemoryStorage<MemoryManager>&) = delete;
            WriteOnlyMemoryStorage<MemoryManager>& operator=(const WriteOnlyMemoryStorage<MemoryManager>&) = delete;

            WriteOnlyMemoryStorage(WriteOnlyMemoryStorage<MemoryManager>&&) = delete;
            WriteOnlyMemoryStorage<MemoryManager>& operator=(WriteOnlyMemoryStorage<MemoryManager>&&) = delete;

            bool put(bool value)
            {
                auto ptr = memoryManager_.getPtr(sizeof(bool));
                if (ptr == nullptr)
                    return false;
                *ptr = (value ? 1 : 0);
                return true;
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 1, int>::type = 0>
            bool put(T value)
            {
                auto ptr = memoryManager_.getPtr(sizeof(T));
                if (ptr == nullptr)
                    return false;
                *ptr = value;
                return true;
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 2, int>::type = 0>
            bool put(T value)
            {
                auto to = memoryManager_.getPtr(sizeof(T));
                if (to == nullptr)
                    return false;
                auto from = reinterpret_cast<char*>(&value);
                to[0] = from[0];
                to[1] = from[1];
                return true;
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 4, int>::type = 0>
            bool put(T value)
            {
                auto to = memoryManager_.getPtr(sizeof(T));
                if (to == nullptr)
                    return false;
                auto from = reinterpret_cast<char*>(&value);
                to[0] = from[0];
                to[1] = from[1];
                to[2] = from[2];
                to[3] = from[3];
                return true;
            }

            template <class T, typename std::enable_if<std::is_arithmetic<T>::value && sizeof(T) == 8, int>::type = 0>
            bool put(T value)
            {
                auto to = memoryManager_.getPtr(sizeof(T));
                if (to == nullptr)
                    return false;
                auto from = reinterpret_cast<char*>(&value);
                to[0] = from[0];
                to[1] = from[1];
                to[2] = from[2];
                to[3] = from[3];
                to[4] = from[4];
                to[5] = from[5];
                to[6] = from[6];
                to[7] = from[7];
                return true;
            }

            template <class T, typename std::enable_if<std::is_enum<T>::value, int>::type = 0>
            bool put(T value)
            {
                return put(static_cast<std::underlying_type_t<T>>(value));
            }

            bool put(const char* data, size_t size)
            {
                auto to = memoryManager_.getPtr(size);
                if (to == nullptr)
                    return false;
                std::copy(data, data + size, to);
                return true;
            }

            const char* data() const noexcept
            {
                return memoryManager_.data();
            }

            size_t size() const noexcept
            {
                return memoryManager_.size();
            }

        private:
            MemoryManager memoryManager_;
        };
    }

    class FixedSizeWriteOnlyMemoryStorage final
        : public details::WriteOnlyMemoryStorage<details::FixedSizeMemoryManager>
    {
    public:
        FixedSizeWriteOnlyMemoryStorage(char* data, size_t size) noexcept
            : details::WriteOnlyMemoryStorage<details::FixedSizeMemoryManager>(details::FixedSizeMemoryManager(data, size))
        {
        }
    };

    class ResizeableWriteOnlyMemoryStorage final
        : public details::WriteOnlyMemoryStorage<details::ResizeableMemoryManager>
    {
    public:
        explicit ResizeableWriteOnlyMemoryStorage(size_t initialSize = 1024, size_t maxSize = std::numeric_limits<uint32_t>::max())
            : details::WriteOnlyMemoryStorage<details::ResizeableMemoryManager>(details::ResizeableMemoryManager(initialSize, maxSize))
        {
            assert(initialSize <= maxSize);
        }
    };
}
