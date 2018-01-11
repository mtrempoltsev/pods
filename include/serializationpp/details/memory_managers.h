#pragma once

#include <vector>

namespace spp
{
    namespace details
    {
        class FixedSizeMemoryManager final
        {
        public:
            FixedSizeMemoryManager(char* data, size_t size) noexcept
                : maxSize_(size)
                , data_(data)
                , pos_(0)
            {
            }

            FixedSizeMemoryManager(const FixedSizeMemoryManager&) = delete;
            FixedSizeMemoryManager& operator=(const FixedSizeMemoryManager&) = delete;

            FixedSizeMemoryManager(FixedSizeMemoryManager&&) = default;
            FixedSizeMemoryManager& operator=(FixedSizeMemoryManager&&) = default;

            char* getPtr(size_t size) noexcept
            {
                if (pos_ + size > maxSize_)
                    return nullptr;
                auto ptr = data_ + pos_;
                pos_ += size;
                return ptr;
            }

            const char* data() const noexcept
            {
                return data_;
            }

            size_t size() const noexcept
            {
                return pos_;
            }

        private:
            const size_t maxSize_;
            char* const data_;
            size_t pos_;
        };

        class ResizeableMemoryManager final
        {
        public:
            ResizeableMemoryManager(size_t initialSize, size_t maxSize)
                : maxSize_(maxSize)
            {
                data_.reserve(initialSize);
            }

            ResizeableMemoryManager(const FixedSizeMemoryManager&) = delete;
            ResizeableMemoryManager& operator=(const ResizeableMemoryManager&) = delete;

            ResizeableMemoryManager(ResizeableMemoryManager&&) = default;
            ResizeableMemoryManager& operator=(ResizeableMemoryManager&&) = default;

            char* getPtr(size_t size)
            {
                const auto oldSize = data_.size();
                const auto newSize = oldSize + size;
                if (newSize > maxSize_)
                    return false;
                data_.resize(newSize);
                return data_.data() + oldSize;
            }

            const char* data() const noexcept
            {
                return data_.data();
            }

            size_t size() const noexcept
            {
                return data_.size();
            }

        private:
            const size_t maxSize_;
            std::vector<char> data_;
        };
    }
}
