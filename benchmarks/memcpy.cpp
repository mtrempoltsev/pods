#include <iostream>
#include <chrono>

#include <pods/binary.h>
#include <pods/msgpack.h>
#include <pods/buffers.h>

#include "memcpy_data.h"

const auto N = 10000;
const auto R = 50;

class Timer
{
public:
    void start()
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    std::chrono::microseconds::rep stop()
    {
        const auto finish = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(finish - start_).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_;
};

void printSpeed(size_t n, size_t size, std::chrono::microseconds::rep us)
{
    std::cout << "    total: " << n * size / (1024 * 1024) << " Mb" << '\n';
    std::cout << "    total time: " << us / 1000 << " ms" << '\n';
    if (us == 0)
    {
        us = 1;
    }
    const size_t mb = 1024 * 1024;
    const size_t sec = 1000000;
    std::cout << "    speed: " << (n * size) / static_cast<size_t>(us) * mb / sec << " Mb/s" << '\n';
}

template <class Serializer, class T>
size_t getSerializedSize(const T& data)
{
    pods::ResizableOutputBuffer out;
    Serializer serializer(out);
    if (serializer.save(data) != pods::Error::NoError)
    {
        std::cerr << "broken serializer\n";
        return EXIT_FAILURE;
    }
    return out.size();
}

template <class Serializer, class Deserializer, class T>
void benchmark(T& data, char* buffer, size_t size)
{
    Timer timer;

    {
        std::cout << "\nserialization" << std::endl;
        timer.start();
        for (size_t r = 0; r < R; ++r)
        {
            for (size_t i = 0; i < N; ++i)
            {
                pods::OutputBuffer out(buffer + i * size, size);
                Serializer serializer(out);
                if (serializer.save(data) != pods::Error::NoError)
                {
                    std::cerr << "broken serializer\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        printSpeed(N * R, size, timer.stop());
    }

    {
        std::cout << "\ndeserialization" << std::endl;
        timer.start();
        for (size_t r = 0; r < R; ++r)
        {
            for (size_t i = 0; i < N; ++i)
            {
                pods::InputBuffer in(buffer + i * size, size);
                Deserializer deserializer(in);
                if (deserializer.load(data) != pods::Error::NoError)
                {
                    std::cerr << "broken deserializer\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        printSpeed(N * R, size, timer.stop());
    }
}

int main()
{
    const auto size = sizeof(Data);
    std::cout << "data size: " << size << '\n';

    Data data = {};

    const auto serializedSize = getSerializedSize<pods::BinarySerializer<pods::ResizableOutputBuffer>>(data);
    std::cout << "serialized data size: " << serializedSize << '\n';

    const auto msgpackSerializedSize = getSerializedSize<pods::MsgPackSerializer<pods::ResizableOutputBuffer>>(data);
    std::cout << "msgpack serialized data size: " << msgpackSerializedSize << '\n';

    const auto maxSize = std::max(size, std::max(serializedSize, msgpackSerializedSize));
    auto buffer = std::make_unique<char[]>(maxSize * N);

    {
        std::cout << "\nmemcpy" << std::endl;

        Timer timer;
        timer.start();

        for (size_t r = 0; r < R; ++r)
        {
            for (size_t i = 0; i < N; ++i)
            {
                memcpy(buffer.get() + i * size, &data, size);
            }
        }
        printSpeed(N * R, size, timer.stop());
    }

    std::cout << "\nPODS format";
    benchmark<pods::MsgPackSerializer<pods::OutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>(data, buffer.get(), serializedSize);

    std::cout << "\nMsgPack format";
    benchmark<pods::MsgPackSerializer<pods::OutputBuffer>, pods::MsgPackDeserializer<pods::InputBuffer>>(data, buffer.get(), msgpackSerializedSize);

    return 0;
}
