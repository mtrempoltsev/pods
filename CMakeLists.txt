cmake_minimum_required(VERSION 3.4)

project(pods)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

get_filename_component(PODS_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

set(PODS_INCLUDE "${PODS_ROOT}/include")
include_directories(${PODS_INCLUDE})

set(PODS_HEADERS "${PODS_INCLUDE}/pods")

set(HEADERS
    ${PODS_HEADERS}/errors.h
    ${PODS_HEADERS}/binary_serializer.h
    ${PODS_HEADERS}/json_serializer.h
    ${PODS_HEADERS}/memory_storage.h
    ${PODS_HEADERS}/pods.h
    ${PODS_HEADERS}/stream_storage.h
    ${PODS_HEADERS}/types.h

    ${PODS_HEADERS}/details/memory_managers.h
    ${PODS_HEADERS}/details/settings.h
    ${PODS_HEADERS}/details/utils.h
    )

add_library(${PROJECT_NAME} STATIC ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(PODS_EXAMPLES ${PROJECT_NAME}_examples)
add_subdirectory(examples)

set(PODS_TEST_PROJECT test_${PROJECT_NAME})
add_test(NAME ${PODS_TEST_PROJECT} COMMAND ${PODS_TEST_PROJECT})
add_subdirectory(tests)
enable_testing()
