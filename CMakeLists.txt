cmake_minimum_required(VERSION 3.4)

project(pods)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4")
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
elseif(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wpedantic --coverage")
endif()

get_filename_component(PODS_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

include_directories("${PODS_ROOT}/externals/rapidjson/include/")

set(PODS_INCLUDE "${PODS_ROOT}/include")
include_directories(${PODS_INCLUDE})

set(PODS_HEADERS "${PODS_INCLUDE}/pods")

set(HEADERS
    ${PODS_HEADERS}/buffers.h
    ${PODS_HEADERS}/errors.h
    ${PODS_HEADERS}/binary_serializer.h
    ${PODS_HEADERS}/json_serializer.h
    ${PODS_HEADERS}/pods.h
    ${PODS_HEADERS}/streams.h
    ${PODS_HEADERS}/types.h

    ${PODS_HEADERS}/details/base64.h
    ${PODS_HEADERS}/details/binary.h
    ${PODS_HEADERS}/details/memory_managers.h
    ${PODS_HEADERS}/details/rapidjson_wrappers.h
    ${PODS_HEADERS}/details/settings.h
    ${PODS_HEADERS}/details/utils.h
    )

if(MSVC)
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
