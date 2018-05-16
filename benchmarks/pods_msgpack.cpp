#include <chrono>
#include <iostream>

#include <pods/msgpack.h>
#include <pods/buffers.h>

#include "msgpack_data.h"

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

void printSummary(size_t n, size_t size, std::chrono::microseconds::rep us)
{
    const size_t mb = 1024 * 1024;
    const size_t sizeMb = n * size / mb;
    std::cout << "    total: " << sizeMb << " Mb" << '\n';

    size_t timeMs = us / 1000;
    if (timeMs == 0)
    {
        timeMs = 1;
    }
    std::cout << "    total time: " << timeMs << " ms" << '\n';

    const size_t speed = sizeMb * 1000 / timeMs;
    std::cout << "    speed: " << speed << " Mb/s" << '\n';
}

int main()
{
    Data data;
    init(&data);

    pods::OutputBuffer out(MaxSerializedSize);
    pods::MsgPackSerializer<decltype(out)> serializer(out);

    std::cout << "trying to serialize... ";

    const auto ok = (serializer.save(data) == pods::Error::NoError);
    if (!ok)
    {
        std::cout << "FAILURE\n";
        return 1;
    }

    const auto size = out.size();

    std::cout << "ok\n";
    std::cout << "archive size: " << size << " bytes\n";

    std::cout << "serialize " << Repeats << " times...\n";

    Timer timer;

    timer.start();

    for (int i = 0; i < Repeats; ++i)
    {
        out.clear();

        const auto ok = (serializer.save(data) == pods::Error::NoError);
        if (!ok)
        {
            std::cout << "FAILURE\n";
            return 1;
        }
    }

    printSummary(Repeats, size, timer.stop());

    std::cout << "deserialize " << Repeats << " times...\n";

    timer.start();

    for (int i = 0; i < Repeats; ++i)
    {
        pods::InputBuffer in(out.data(), out.size());
        pods::MsgPackDeserializer<decltype(in)> deserializer(in);

        const auto ok = (deserializer.load(data) == pods::Error::NoError);
        if (!ok)
        {
            std::cout << "FAILURE\n";
            return 1;
        }
    }

    printSummary(Repeats, size, timer.stop());

    destroy(&data);

    return 0;
}
