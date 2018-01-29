#include <string>

#include <pods/pods.h>
#include <pods/memory_storage.h>

struct Server
{
    bool isDefault;
    std::string name;
    std::string publicKey;
    std::string address;
    uint16_t port;

    PODS_SERIALIZABLE(1,
        PODS_OPT(isDefault),
        PODS_MDR(name),
        PODS_MDR(publicKey),
        PODS_MDR(address),
        PODS_MDR(port))
};

struct ServerList
{
    std::vector<Server> servers =
    {
        Server { true, "Main", "key1", "addr1", 2017 },
        Server { false, "Secondary", "key2", "addr2", 2018 }
    };

    uint32_t clientId = 100;

    PODS_SERIALIZABLE(1,
        PODS_MDR(servers),
        PODS_MDR(clientId))
};

int main(int argc, char* argv[])
{
    bool success = false;

    const ServerList original;

    pods::ResizeableWriteOnlyMemoryStorage out;
    pods::BinarySerializer<decltype(out)> serializer(out);
    success = (serializer.save(original) == pods::Error::NoError);

    ServerList loaded;
    loaded.servers.clear();
    loaded.clientId = 0;

    pods::ReadOnlyMemoryStorage in(out.data(), out.size());
    pods::BinaryDeserializer<decltype(in)> deserializer(in);
    success = (deserializer.load(loaded) == pods::Error::NoError);

    return 0;
}
