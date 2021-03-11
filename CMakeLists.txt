cmake_minimum_required(VERSION 2.8)
project(bs_sm4)

include_directories(./include)
aux_source_directory(./src DIR_SRCS)

add_executable(bs_sm4_tset ${DIR_SRCS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)