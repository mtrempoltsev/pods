#include <iostream>
#include <sstream>

#include <pods/pods.h>
#include <pods/msgpack.h>
#include <pods/streams.h>

// just a struct for serialization
struct Server
{
    std::string address;        // no default value
    uint16_t port = 8080;       // default value

    PODS_SERIALIZABLE(
        PODS_MDR(address),      // mandatory field
        PODS_OPT(port))         // optional field
};

int main(int /*argc*/, char** /*argv*/)
{
    const Server original;

    std::stringstream buffer;

    pods::OutputStream out(buffer);
    pods::MsgPackSerializer<decltype(out)> serializer(out);
    if (serializer.save(original) != pods::Error::NoError)
    {
        std::cerr << "serialization error\n";
        return EXIT_FAILURE;
    }

    Server loaded = {};

    pods::InputStream in(buffer);
    pods::MsgPackDeserializer<decltype(in)> deserializer(in);
    if (deserializer.load(loaded) != pods::Error::NoError)
    {
        std::cerr << "deserialization error\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
