project(${PODS_EXAMPLES})

include_directories(${PODS_INCLUDE})

set(HEADERS
   )

set(SOURCES
    simple.cpp
    )

add_executable(${PODS_EXAMPLES} ${HEADERS} ${SOURCES})
