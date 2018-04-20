project(${PODS_TEST_PROJECT})

if(PODS_BUILD_GTEST)
    set(PODS_GTEST_ROOT "${PODS_ROOT}/externals/googletest/googletest")
    include_directories(${PODS_GTEST_ROOT}/include)
    include_directories(${PODS_GTEST_ROOT})

    set(PODS_GOOGLETEST_SOURCES
        ${PODS_GTEST_ROOT}/src/gtest-all.cc
        ${PODS_GTEST_ROOT}/src/gtest_main.cc
        )

    add_library(googletest ${PODS_GOOGLETEST_SOURCES})
    set(GTEST_BOTH_LIBRARIES googletest)
else()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

include_directories(${PODS_INCLUDE})

set(HEADERS
    data.h
    stl_data.h
    stl_map_data.h
    storage_data.h
    versioning_data.h
    )

set(SOURCES
    test_base64.cpp
    test_binary_serializer.cpp
    test_binary_versioning.cpp
    test_buffer.cpp
    test_endianness.cpp
    test_json_serializer.cpp
    test_json_versioning.cpp
    test_memory_managers.cpp
    test_rapidjson_wrapper.cpp
    test_resizeable_buffer.cpp
    test_sax_handlers.cpp
    test_stl_map_serialization.cpp
    test_stl_serialization.cpp
    test_stream.cpp
    main.cpp
    )

add_executable(${PODS_TEST_PROJECT} ${HEADERS} ${SOURCES})

find_package(Threads)

target_link_libraries(${PODS_TEST_PROJECT}
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )
