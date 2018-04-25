cmake_minimum_required(VERSION 3.4)

project(pods)

set(CMAKE_CXX_STANDARD 14)

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if (IS_BIG_ENDIAN)
    add_definitions(-DPODS_BIG_ENDIAN)
else()
    add_definitions(-DPODS_LITTLE_ENDIAN)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4")
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic -Wconversion")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wconversion")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic -Wconversion")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic -Wconversion --coverage")
endif()

get_filename_component(PODS_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

include_directories("${PODS_ROOT}/externals/rapidjson/include/")

set(PODS_INCLUDE "${PODS_ROOT}/include")
include_directories(${PODS_INCLUDE})

set(PODS_HEADERS "${PODS_INCLUDE}/pods")

set(PODS_PUBLIC_HEADERS
    ${PODS_HEADERS}/buffers.h
    ${PODS_HEADERS}/errors.h
    ${PODS_HEADERS}/binary.h
    ${PODS_HEADERS}/json.h
    ${PODS_HEADERS}/msgpack.h
    ${PODS_HEADERS}/pods.h
    ${PODS_HEADERS}/streams.h
    ${PODS_HEADERS}/types.h
    )

set(PODS_DETAILS_HEADERS
    ${PODS_HEADERS}/details/base64.h
    ${PODS_HEADERS}/details/binary_wrappers.h
    ${PODS_HEADERS}/details/deserializer.h
    ${PODS_HEADERS}/details/endianness.h
    ${PODS_HEADERS}/details/names.h
    ${PODS_HEADERS}/details/rapidjson_wrappers.h
    ${PODS_HEADERS}/details/serializer.h
    ${PODS_HEADERS}/details/settings.h
    ${PODS_HEADERS}/details/utils.h
    )

set(PODS_FORMATS_HEADERS
    ${PODS_HEADERS}/details/formats/binary_input.h
    ${PODS_HEADERS}/details/formats/binary_output.h
    ${PODS_HEADERS}/details/formats/json_input.h
    ${PODS_HEADERS}/details/formats/json_output.h
    ${PODS_HEADERS}/details/formats/msgpack_aux.h
    ${PODS_HEADERS}/details/formats/msgpack_input.h
    ${PODS_HEADERS}/details/formats/msgpack_output.h
    )

set(HEADERS
    ${PODS_PUBLIC_HEADERS}
    ${PODS_DETAILS_HEADERS}
    ${PODS_FORMATS_HEADERS}
    )

if(MSVC)
    source_group("Header Files" FILES "${PODS_PUBLIC_HEADERS}")
    source_group("Header Files\\details" FILES "${PODS_DETAILS_HEADERS}")
    source_group("Header Files\\formats" FILES "${PODS_FORMATS_HEADERS}")
    add_custom_target(libpods SOURCES ${HEADERS})
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE ${HEADERS})

set(PODS_EXAMPLES ${PROJECT_NAME}_examples)
add_subdirectory(examples)

set(PODS_TEST_PROJECT test_${PROJECT_NAME})
add_test(NAME ${PODS_TEST_PROJECT} COMMAND ${PODS_TEST_PROJECT})
add_subdirectory(tests)
enable_testing()

add_executable(benchmark ${PODS_ROOT}/benchmarks/memcpy.cpp)
