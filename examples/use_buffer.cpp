#include <iostream>

#include <pods/pods.h>
#include <pods/binary.h>
#include <pods/buffers.h>

struct Server
{
    std::string address = "localhost";  // this is default value
    uint16_t port = 8080;               // this is default value

    PODS_SERIALIZABLE(
        1,                      // this is version
        PODS_MDR(address),      // this field is mandatory
        PODS_OPT(port))         // this field is optional
};

int main(int /*argc*/, char** /*argv*/)
{
    const Server original;

    const size_t BufferSize = 1024;

    pods::OutputBuffer out(BufferSize);
    pods::BinarySerializer<decltype(out)> serializer(out);
    if (serializer.save(original) != pods::Error::NoError)
    {
        std::cerr << "serialization error\n";
        return EXIT_FAILURE;
    }

    Server loaded;
    loaded.address = "";
    loaded.port = 0;

    pods::InputBuffer in(out.data(), out.size());
    pods::BinaryDeserializer<decltype(in)> deserializer(in);
    if (deserializer.load(loaded) != pods::Error::NoError)
    {
        std::cerr << "deserialization error\n";
        return EXIT_FAILURE;
    }

    std::cout << "archive size: " << out.size() << '\n';

    return EXIT_SUCCESS;
}
