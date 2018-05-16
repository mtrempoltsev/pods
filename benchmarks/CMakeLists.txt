set(PODS_BENCHMARKS_ROOT ${PODS_ROOT}/benchmarks)

set(PODS_BENCHMARKS_DATA ${PODS_BENCHMARKS_ROOT}/data.h)

add_executable(memcpy ${PODS_BENCHMARKS_ROOT}/memcpy.cpp ${PODS_BENCHMARKS_ROOT}/memcpy_data.h)
set(MSGPUCK msgpuck)
add_executable(bench_msgpuck ${PODS_BENCHMARKS_ROOT}/msgpuck.c ${PODS_BENCHMARKS_ROOT}/msgpack_data.h)
target_link_libraries(bench_msgpuck msgpuck)
add_executable(pods_msgpack ${PODS_BENCHMARKS_ROOT}/pods_msgpack.cpp ${PODS_BENCHMARKS_ROOT}/msgpack_data.h)
