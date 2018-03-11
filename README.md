[![Build Status](https://travis-ci.org/mtrempoltsev/pods.svg?branch=master)](https://travis-ci.org/mtrempoltsev/pods#)
[![Build status](https://ci.appveyor.com/api/projects/status/99x21w4i19tj4beu/branch/master?svg=true)](https://ci.appveyor.com/project/mtrempoltsev/pods/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/mtrempoltsev/pods/badge.svg?branch=master)](https://coveralls.io/github/mtrempoltsev/pods?branch=master)

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

#### Comparison with memcpy

> MacBook Pro, 2017
> 3.5 GHz Intel Core i7
> 16 Gb 2133 MHz LPDDR3

```
data size: 11520
serialized data size: 11632

memcpy
    total: 27465 Mb
    total time: 2396.19 ms
    speed: 11461.9 Mb/s

serialization
    total: 27732 Mb
    total time: 3502.93 ms
    speed: 7916.8 Mb/s

deserialization
    total: 27732 Mb
    total time: 3200.15 ms
    speed: 8665.83 Mb/s
```

#### Comparison with other libraries

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

### Binary serialization

```c++
#include <iostream>

#include <pods/pods.h>
#include <pods/binary_serializer.h>
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

    pods::ResizableOutputBuffer out;
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

    if (original.address != loaded.address
        || original.port != loaded.port)
    {
        std::cerr << "corrupted archive\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

### JSON serialization

```c++
#include <iostream>

#include <pods/pods.h>
#include <pods/json_serializer.h>
#include <pods/buffers.h>

struct Server
{
    std::string address;
    uint16_t port;

    PODS_SERIALIZABLE(
        1,                      // this is version
        PODS_MDR(address),      // this field is mandatory
        PODS_OPT(port))         // this field is optional
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
        5,
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
```

##### Output:

```
{
    "version": 5,
    "servers": {
        "version": 1,
        "data": [
            {
                "address": "localhost",
                "port": 8080
            },
            {
                "address": "my.com",
                "port": 2018
            }
        ]
    }
}
```

### Standard streams

```c++
#include <iostream>
#include <sstream>

#include <pods/pods.h>
#include <pods/binary_serializer.h>
#include <pods/streams.h>

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

    std::stringstream buffer;

    pods::OutputStream out(buffer);
    pods::BinarySerializer<decltype(out)> serializer(out);
    if (serializer.save(original) != pods::Error::NoError)
    {
        std::cerr << "serialization error\n";
        return EXIT_FAILURE;
    }

    Server loaded;
    loaded.address = "";
    loaded.port = 0;

    pods::InputStream in(buffer);
    pods::BinaryDeserializer<decltype(in)> deserializer(in);
    if (deserializer.load(loaded) != pods::Error::NoError)
    {
        std::cerr << "deserialization error\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

```
