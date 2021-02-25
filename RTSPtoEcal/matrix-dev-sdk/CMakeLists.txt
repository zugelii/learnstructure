cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_src
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
)


link_directories(${CMAKE_SOURCE_DIR}/lib})

add_executable(${PROJECT_NAME} ${PROJECT_src})

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs features2d flann calib3d)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libmatrix-dev-sdk.so)
