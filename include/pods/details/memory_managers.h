#pragma once

namespace pods
{
    namespace details
    {
        struct Buffer
        {
            explicit Buffer(size_t size) noexcept
                : ptr(static_cast<char*>(malloc(size)))
            {
            }

            ~Buffer() noexcept
            {
                free(ptr);
            }

            Buffer(const Buffer&) = delete;
            Buffer& operator=(const Buffer&) = delete;

            Buffer(Buffer&& movied) noexcept
                : ptr(movied.ptr)
            {
                movied.ptr = nullptr;
            }

            Buffer& operator=(Buffer&& movied) noexcept
            {
                if (this != &movied)
                {
                    if (ptr != nullptr)
                    {
                        free(ptr);
                        ptr = nullptr;
                    }

                    std::swap(ptr, movied.ptr);
                }

                return *this;
            }

            bool resize(size_t newSize) noexcept
            {
                auto newPtr = realloc(ptr, newSize);
                if (newPtr == nullptr)
                {
                    return false;
                }

                ptr = static_cast<char*>(newPtr);
                return true;
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

            void reset()
            {
                pos_ = 0;
            }

            size_t available() const noexcept
            {
                assert(maxSize_ >= pos_);
                return maxSize_ - pos_;
            }

        private:
            const size_t maxSize_;
            size_t pos_;
            Buffer data_;
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
                if (data_.ptr == nullptr)
                {
                    throw std::bad_alloc();
                }
            }

            ResizeableMemoryManager(const FixedSizeMemoryManager&) = delete;
            ResizeableMemoryManager& operator=(const ResizeableMemoryManager&) = delete;

            ResizeableMemoryManager(ResizeableMemoryManager&&) = default;
            ResizeableMemoryManager& operator=(ResizeableMemoryManager&&) = default;

            char* getPtr(size_t size) noexcept
            {
                const auto newPos = pos_ + size;

                if (newPos <= maxSize_)
                {
                    if (newPos > capacity_)
                    {
                        if (!increaseSize(newPos))
                        {
                            return nullptr;
                        }
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

            void reset()
            {
                pos_ = 0;
            }

            size_t available() const noexcept
            {
                assert(maxSize_ >= pos_);
                return maxSize_ - pos_;
            }

        private:
            bool increaseSize(size_t required) noexcept
            {
                assert(capacity_ < required);

                auto newCapacity = capacity_ * 2;
                while (newCapacity < required)
                {
                    newCapacity *= 2;
                }

                if (data_.resize(newCapacity))
                {
                    capacity_ = newCapacity;
                    return true;
                }

                return false;
            }

        private:
            const size_t maxSize_;
            size_t pos_;
            size_t capacity_;
            Buffer data_;
        };
    }
}
