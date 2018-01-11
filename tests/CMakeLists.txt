project(${SERIALIZATIONPP_TEST_PROJECT})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(${SERIALIZATIONPP_INCLUDE})

set(SOURCES
    test_fixed_memory_storage.cpp
    test_resizeable_memory_storage.cpp
    main.cpp
    )

add_executable(${SERIALIZATIONPP_TEST_PROJECT} ${SOURCES})

target_link_libraries(${SERIALIZATIONPP_TEST_PROJECT} ${GTEST_BOTH_LIBRARIES})
