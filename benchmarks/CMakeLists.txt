set(PODS_BENCHMARKS_ROOT ${PODS_ROOT}/benchmarks)

set(PODS_BENCHMARKS_DATA ${PODS_BENCHMARKS_ROOT}/data.h)

add_executable(memcpy ${PODS_BENCHMARKS_ROOT}/memcpy.cpp ${PODS_BENCHMARKS_ROOT}/memcpy_data.h)

if(PODS_BENCH_MSGPUCK)
    set(MSGPUCK_DATA ${PODS_BENCHMARKS_ROOT}/msgpack_data.h)
    add_executable(orig_msgpuck ${PODS_BENCHMARKS_ROOT}/orig_msgpuck.c ${MSGPUCK_DATA})
    target_link_libraries(orig_msgpuck msgpuck)

    add_executable(pods_msgpack ${PODS_BENCHMARKS_ROOT}/pods_msgpack.cpp ${MSGPUCK_DATA})
endif()
