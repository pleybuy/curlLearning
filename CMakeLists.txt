cmake_minimum_required(VERSION 3.14)
project(curlTest)

set(CMAKE_CXX_STANDARD 11)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        G:\\Develop\\common\\include
        #/Users/san/common/include
        #C:\\develop\\common\\include
        G:\\Develop\\common\\include\\x86_64-linux-gnu
        #C:\\develop\\common\\include\\x86_64-linux-gnu
)
aux_source_directory(. SRC_LIST)
add_executable(curlTest ${SRC_LIST})