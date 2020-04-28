cmake_minimum_required(VERSION 3.12.0)
project(readrtsp VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 14) #for std::make_unique


find_package(OpenCV REQUIRED)
#find_package(ffmpeg REQUIRED)


file(GLOB SRC_FILE_LIST ./*.cpp RSICase/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILE_LIST})


target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
