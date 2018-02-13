project(${PODS_EXAMPLES})

include_directories(${PODS_INCLUDE})

set(SOURCES
    binary.cpp
    json.cpp
    use_buffer.cpp
    streams.cpp
    )

add_executable(binary binary.cpp)
add_executable(json json.cpp)
add_executable(use_buffer use_buffer.cpp)
add_executable(streams streams.cpp)
