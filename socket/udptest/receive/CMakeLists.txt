cmake_minimum_required(VERSION 3.12.0)
project(udprev VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

file(GLOB SRC_FILE_LIST ./*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILE_LIST})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
