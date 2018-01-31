# Plain Old Data Serializer (PODS)

![](doc/images/pod.png)

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

## Benchmarks

```
performing 100000 iterations

thrift-binary: version = 0.10.0
thrift-binary: size = 17017 bytes
thrift-binary: time = 2259 milliseconds

protobuf: version = 3001000
protobuf: size = 16116 bytes
protobuf: time = 2797 milliseconds

capnproto: version = 6001
capnproto: size = 17768 bytes
capnproto: time = 486 milliseconds

boost: version = 106200
boost: size = 17470 bytes
boost: time = 1365 milliseconds

msgpack: version = 2.1.3
msgpack: size = 13402 bytes
msgpack: time = 3638 milliseconds

cereal: size = 17416 bytes
cereal: time = 1034 milliseconds

avro: size = 16384 bytes
avro: time = 4780 milliseconds

flatbuffers: size = 17632 bytes
flatbuffers: time = 433 milliseconds

yas: version = 6.0.2
yas: size = 17416 bytes
yas: time = 317 milliseconds

pods: size = 17012 bytes
pods: time = 235 milliseconds
```

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
