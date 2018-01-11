#pragma once

namespace spp
{
    template <class Storage>
    class FastBinarySerializator final
    {
    public:
        explicit FastBinarySerializator(Storage& storage) noexcept
            : storage_(storage)
        {
        }

        FastBinarySerializator(const FastBinarySerializator&) = delete;
        FastBinarySerializator& operator=(const FastBinarySerializator&) = delete;

        template <class T>
        bool save(T& data, Version version)
        {
            storage_.put(version);
            data.serialize(*this, version);
        }

    private:
        Storage& storage_;
    };

    template <class Storage>
    class FastBinaryDeserializator final
    {
    public:
        explicit FastBinaryDeserializator(Storage& storage) noexcept
            : storage_(storage)
        {
        }

        FastBinaryDeserializator(const FastBinaryDeserializator&) = delete;
        FastBinaryDeserializator& operator=(const FastBinaryDeserializator&) = delete;

        template <class T>
        bool load(T& data)
        {
            Version version = 0;
            storage_.get(version);
            data.serialize(*this, version);
        }

    private:
        Storage& storage_;
    };
}
