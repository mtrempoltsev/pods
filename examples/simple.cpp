#include <pods/pods.h>
#include <pods/binary_serializer.h>
#include <pods/memory_storage.h>

struct Server
{
    bool isDefault;
    std::string name;
    std::string publicKey;
    std::string address;
    uint16_t port;

    PODS_SERIALIZABLE(
        1,                    // this is the version id
        PODS_OPT(isDefault),  // this field is optional
        PODS_MDR(name),       // this field is mandatory
        PODS_MDR(publicKey),
        PODS_MDR(address),
        PODS_MDR(port))
};

struct ServerList
{
    std::vector<Server> servers =
    {
        // this is default values
        Server { true, "Main", "key1", "addr1", 2017 },
        Server { false, "Secondary", "key2", "addr2", 2018 }
    };

    uint32_t clientId = 100;

    PODS_SERIALIZABLE(
        1,
        PODS_MDR(servers),
        PODS_MDR(clientId))
};

int main(int /*argc*/, char** /*argv*/)
{
    const ServerList original;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<decltype(out)> serializer(out);
    if (serializer.save(original) != pods::Error::NoError)
    {
        return -1;
    }

    ServerList loaded;
    loaded.servers.clear();
    loaded.clientId = 0;

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<decltype(in)> deserializer(in);
    if (deserializer.load(loaded) != pods::Error::NoError)
    {
        return -1;
    }

    return 0;
}

