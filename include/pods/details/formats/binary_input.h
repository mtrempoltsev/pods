#pragma once

#include "../utils.h"

#include "../../errors.h"
#include "../../types.h"

namespace pods
{
    namespace details
    {
        template <class Storage>
        class BinaryInput final
        {
        public:
            explicit BinaryInput(Storage& storage) noexcept
                : storage_(storage)
            {
            }

            BinaryInput(const BinaryInput<Storage>&) = delete;
            BinaryInput& operator=(const BinaryInput<Storage>&) = delete;

            Error startDeserialization() noexcept
            {
                return Error::NoError;
            }

            Error endDeserialization() noexcept
            {
                return Error::NoError;
            }

            Error checkName(const char*) noexcept
            {
                return Error::NoError;
            }

            Error startObject() noexcept
            {
                return Error::NoError;
            }

            Error endObject() noexcept
            {
                return Error::NoError;
            }

            Error startArray(Size& size)
            {
                return storage_.get(size);
            }

            Error endArray() noexcept
            {
                return Error::NoError;
            }

            Error startMap(Size& size)
            {
                return startArray(size);
            }

            Error endMap() noexcept
            {
                return endArray();
            }

            template <class T>
            Error load(T& value)
            {
                return storage_.get(value);
            }

            Error load(bool& value)
            {
                Bool n = False;
                PODS_SAFE_CALL(storage_.get(n));
                switch (n)
                {
                case False:
                    value = false;
                    return Error::NoError;
                case True:
                    value = true;
                    return Error::NoError;
                }
                return Error::CorruptedArchive;
            }

            Error load(std::string& value)
            {
                Size size = 0;
                PODS_SAFE_CALL(storage_.get(size));
                value.resize(size);
                return storage_.get(const_cast<char*>(value.data()), size);
            }

            Error loadBlob(char* data, size_t size)
            {
                Size actualSize = 0;
                PODS_SAFE_CALL(storage_.get(actualSize));
                return size == actualSize
                    ? storage_.get(data, actualSize)
                    : Error::CorruptedArchive;
            }

            template <class Allocator>
            Error loadBlob(const Allocator& allocator)
            {
                Size size = 0;
                PODS_SAFE_CALL(storage_.get(size));
                char* data = nullptr;
                PODS_SAFE_CALL(allocator(data, size));
                return storage_.get(data, size);
            }

        private:
            Storage& storage_;
        };
    }
}
