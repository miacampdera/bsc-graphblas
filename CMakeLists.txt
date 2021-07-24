cmake_minimum_required(VERSION 3.19)
project(bsc_graphblas C)

set(CMAKE_C_STANDARD 99)

add_executable(bsc_graphblas main.c uw_sssp.c w_sssp.c batch_sssp.c map.c)

include_directories(/usr/local/include)

find_library(GRAPHBLAS_LIBRARY graphblas HINTS /usr/local/lib)

target_link_libraries(bsc_graphblas PUBLIC ${GRAPHBLAS_LIBRARY})
