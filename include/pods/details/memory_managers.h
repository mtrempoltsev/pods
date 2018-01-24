#pragma once

namespace pods
{
    namespace details
    {
        struct MemoryHolder
        {
            explicit MemoryHolder(size_t size)
                : ptr(new char[size])
            {
            }

            ~MemoryHolder()
            {
                delete[] ptr;
            }

            MemoryHolder(const MemoryHolder&) = delete;
            MemoryHolder& operator=(const MemoryHolder&) = delete;

            MemoryHolder(MemoryHolder&& movied)
                : ptr(movied.ptr)
            {
                movied.ptr = nullptr;
            }

            MemoryHolder& operator=(MemoryHolder&& movied)
            {
                if (this != &movied)
                {
                    if (ptr != nullptr)
                    {
                        delete[] ptr;
                        ptr = nullptr;
                    }

                    swap(movied);
                }

                return *this;
            }

            void swap(MemoryHolder& other) noexcept
            {
                std::swap(ptr, other.ptr);
            }

            char* ptr;
        };

        class FixedSizeMemoryManager final
        {
        public:
            explicit FixedSizeMemoryManager(size_t size) noexcept
                : maxSize_(size)
                , pos_(0)
                , data_(size)
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
                    auto ptr = data_.ptr + pos_;
                    pos_ += size;
                    return ptr;
                }

                return nullptr;
            }

            const char* data() const noexcept
            {
                return data_.ptr;
            }

            size_t size() const noexcept
            {
                return pos_;
            }

        private:
            const size_t maxSize_;
            size_t pos_;
            MemoryHolder data_;
        };

        class ResizeableMemoryManager final
        {
        public:
            ResizeableMemoryManager(size_t initialSize, size_t maxSize)
                : maxSize_(maxSize)
                , pos_(0)
                , capacity_(initialSize)
                , data_(initialSize)
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

                    const auto ptr = data_.ptr + pos_;
                    pos_ = newPos;
                    return ptr;
                }

                return nullptr;
            }

            const char* data() const noexcept
            {
                return data_.ptr;
            }

            size_t size() const noexcept
            {
                return pos_;
            }

        private:
            void increaseSize()
            {
                const auto newCapacity = capacity_ * 2;
                MemoryHolder newData(newCapacity);
                memcpy(newData.ptr, data_.ptr, pos_);
                data_.swap(newData);
                capacity_ = newCapacity;
            }

        private:
            const size_t maxSize_;
            size_t pos_;
            size_t capacity_;
            MemoryHolder data_;
        };
    }
}
