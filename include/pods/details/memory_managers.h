#pragma once

#include <memory>

namespace pods
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
                if (pos_ + size <= maxSize_)
                {
                    auto ptr = data_ + pos_;
                    pos_ += size;
                    return ptr;
                }

                return nullptr;
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
                , pos_(0)
                , capacity_(initialSize)
                , data_(new char[initialSize])
            {
            }

            ResizeableMemoryManager(const FixedSizeMemoryManager&) = delete;
            ResizeableMemoryManager& operator=(const ResizeableMemoryManager&) = delete;

            ResizeableMemoryManager(ResizeableMemoryManager&&) = default;
            ResizeableMemoryManager& operator=(ResizeableMemoryManager&&) = default;

            char* getPtr(size_t size)
            {
                const auto newPos = pos_ + size;

                if (newPos <= maxSize_)
                {
                    if (newPos > capacity_)
                    {
                        increaseSize();
                    }

                    const auto ptr = data_.get() + pos_;
                    pos_ = newPos;
                    return ptr;
                }

                return nullptr;
            }

            const char* data() const noexcept
            {
                return data_.get();
            }

            size_t size() const noexcept
            {
                return pos_;
            }

        private:
            void increaseSize()
            {
                const auto newCapacity = capacity_ * 2;
                auto newData = std::make_unique<char[]>(newCapacity);
                memcpy(newData.get(), data_.get(), pos_);
                data_.swap(newData);
                capacity_ = newCapacity;
            }

        private:
            const size_t maxSize_;
            size_t pos_;
            size_t capacity_;
            std::unique_ptr<char[]> data_;
        };
    }
}
