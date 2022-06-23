#include <iostream>

#include <pods/pods.h>
#include <pods/json.h>
#include <pods/buffers.h>

// just a struct for serialization
struct Server
{
    std::string address;        // no default value
    uint16_t port = 8080;       // default value

    PODS_SERIALIZABLE(
        PODS_MDR(address),      // mandatory field
        PODS_OPT(port))         // optional field
};

struct ServerList
{
    std::vector<Server> servers =
    {
        // this is default values
        Server { "localhost", 8080 },
        Server { "my.com", 2018 }
    };

    PODS_SERIALIZABLE(
        PODS_MDR(servers))
};

int main(int /*argc*/, char** /*argv*/)
{
    const ServerList original;

    pods::ResizableOutputBuffer out;
    pods::PrettyJsonSerializer<decltype(out)> serializer(out);
    if (serializer.save(original) != pods::Error::NoError)
    {
        std::cerr << "serialization error\n";
        return EXIT_FAILURE;
    }

    ServerList loaded;
    loaded.servers.clear();

    pods::InputBuffer in(out.data(), out.size());
    pods::JsonDeserializer<decltype(in)> deserializer(in);
    if (deserializer.load(loaded) != pods::Error::NoError)
    {
        std::cerr << "deserialization error\n";
        return EXIT_FAILURE;
    }

    const std::string json(out.data(), out.size());
    std::cout << json << '\n';

    return EXIT_SUCCESS;
}
