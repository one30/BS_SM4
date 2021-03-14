cmake_minimum_required(VERSION 2.8)
project(bs_sm4)
# set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O2  -Wall -m64 -mavx -mavx2") -mfloat-abi=softfp 
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -mfpu=neon -O2  -Wall -flax-vector-conversions")

include_directories(./include)
aux_source_directory(./src DIR_SRCS)
# add_executable(bs_sm4_tset ${DIR_SRCS})
add_executable(bs_sm4_tset ./test/neon_test.c ${DIR_SRCS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)