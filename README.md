Plain Old Data Serializer (PODS)

![](doc/images/pod.png)

## How to build

From this directory:
```
mkdir build
cd build
cmake ..
make
```

## Feature List

- header only
- high performance (perhaps binary serialization is the fastest on the Earth)
- optional values
- versioning
- supported archive formats:
  - JSON
  - binary
- serialization from/to:
  - memory buffer
  - resizable memory buffer
  - standard C++ streams

## Using PODS

```c++
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
```
