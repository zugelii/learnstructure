cmake_minimum_required(VERSION 3.12.0)
project(pidtest VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

find_package(eCAL REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SRC_FILE_LIST ./*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILE_LIST})

target_link_libraries(${PROJECT_NAME} eCAL::core ${CMAKE_THREAD_LIBS_INIT} )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
