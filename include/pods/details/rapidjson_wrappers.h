#pragma once

#include "../errors.h"

namespace pods
{
    namespace details
    {
        template <class Storage>
        struct OutputRapidJsonStreamWrapper
        {
            typedef char Ch;

            OutputRapidJsonStreamWrapper(Storage& storage) noexcept
                : storage_(storage)
                , good_(true)
            {
            }

            void Put(Ch c)
            {
                if (storage_.put(c) == Error::NoError)
                {
                    return;
                }

                good_ = false;
            }

            void Flush() noexcept
            {
                storage_.flush();
            }

            bool good() const noexcept
            {
                return good_;
            }

        private:
            Storage& storage_;
            bool good_;
        };

        template <class Storage>
        struct InputRapidJsonStreamWrapper
        {
            typedef char Ch;

            InputRapidJsonStreamWrapper(Storage& storage) noexcept
                : storage_(storage)
                , good_(true)
                , n_(0)
                , peeked_('\0')
            {
            }

            Ch Peek() const noexcept
            {
                if (good_)
                {
                    if (peeked_ != '\0')
                    {
                        return peeked_;
                    }

                    if (storage_.get(peeked_) == Error::NoError)
                    {
                        return peeked_;
                    }
                }

                good_ = false;
                peeked_ = '\0';

                return peeked_;
            }

            Ch Take() noexcept
            {
                if (good_)
                {
                    const auto c = peeked_;
                    peeked_ = '\0';
                    ++n_;
                    return c;
                }

                return '\0';
            }

            size_t Tell() const noexcept
            {
                return n_;
            }

            void Put(Ch) noexcept
            {
                assert(false);
            }

            Ch* PutBegin() noexcept
            {
                assert(false);
                return nullptr;
            }

            size_t PutEnd(Ch*) noexcept
            {
                assert(false);
                return '\0';
            }

            bool good() const noexcept
            {
                return good_;
            }

        private:
            Storage& storage_;
            mutable bool good_;
            size_t n_;
            mutable char peeked_;
        };
    }
}
