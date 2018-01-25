project(${PODS_TEST_PROJECT})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${PODS_INCLUDE})

set(HEADERS
   data.h
   storage_data.h
   )

set(SOURCES
    test_binary_serializer.cpp
    test_fixed_memory_storage.cpp
    test_json_serializer.cpp
    test_memory_managers.cpp
    test_portable_binary_serializer.cpp
    test_resizeable_memory_storage.cpp
    main.cpp
    )

add_executable(${PODS_TEST_PROJECT} ${HEADERS} ${SOURCES})

find_package(Threads)

target_link_libraries(${PODS_TEST_PROJECT}
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )
