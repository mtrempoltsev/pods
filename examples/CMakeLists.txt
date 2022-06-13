project(${PODS_EXAMPLES})

include_directories(${PODS_INCLUDE})

set(SOURCES
    json.cpp
    msgpack.cpp
    use_buffer.cpp
    streams.cpp
    )

add_executable(json json.cpp)
add_executable(msgpack msgpack.cpp)
add_executable(use_buffer use_buffer.cpp)
add_executable(streams streams.cpp)
