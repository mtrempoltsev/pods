#include <iostream>
#include <chrono>

#include <pods/binary.h>
#include <pods/buffers.h>

#include "data.h"

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
    std::cout << "    speed: " << (n * size / (1024 * 1024)) / (us / 1000000) << " Mb/s" << '\n';
}

int main()
{
    const auto size = sizeof(Data);
    std::cout << "data size: " << size << '\n';

    Data data = {};

    pods::ResizableOutputBuffer out1;
    pods::BinarySerializer<decltype(out1)> serializer1(out1);
    if (serializer1.save(data) != pods::Error::NoError)
    {
        std::cerr << "broken serializer\n";
        return EXIT_FAILURE;
    }

    const auto serializedSize = out1.size();
    std::cout << "serialized data size: " << serializedSize << '\n';

    const auto N = 500000;
    const auto R = 5;

    auto buffer = std::make_unique<char[]>(serializedSize * N);

    Timer timer;

    {
        std::cout << "\nmemcpy" << std::endl;
        timer.start();
        for (auto r = 0; r < R; ++r)
        {
            for (auto i = 0; i < N; ++i)
            {
                memcpy(buffer.get() + i * serializedSize, &data, serializedSize);
            }
        }
        printSpeed(N * R, size, timer.stop());
    }

    {
        std::cout << "\nserialization" << std::endl;
        timer.start();
        for (auto r = 0; r < R; ++r)
        {
            for (auto i = 0; i < N; ++i)
            {
                pods::OutputBuffer out2(buffer.get() + i * serializedSize, serializedSize);
                pods::BinarySerializer<decltype(out2)> serializer2(out2);
                if (serializer2.save(data) != pods::Error::NoError)
                {
                    std::cerr << "broken serializer\n";
                    return EXIT_FAILURE;
                }
            }
        }
        printSpeed(N * R, serializedSize, timer.stop());
    }

    {
        std::cout << "\ndeserialization" << std::endl;
        timer.start();
        for (auto r = 0; r < R; ++r)
        {
            for (auto i = 0; i < N; ++i)
            {
                pods::InputBuffer in(buffer.get() + i * serializedSize, serializedSize);
                pods::BinaryDeserializer<decltype(in)> deserializer(in);
                if (deserializer.load(data) != pods::Error::NoError)
                {
                    std::cerr << "broken deserializer\n";
                    return EXIT_FAILURE;
                }
            }
        }
        printSpeed(N * R, serializedSize, timer.stop());
    }

    return 0;
}
