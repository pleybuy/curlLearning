cmake_minimum_required(VERSION 3.15)
project(curlTest)

set(CMAKE_CXX_STANDARD 11)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        G:\\Develop\\common\\include
)
aux_source_directory(. SRC_LIST)
add_executable(curlTest ${SRC_LIST})