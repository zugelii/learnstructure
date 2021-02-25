cmake_minimum_required(VERSION 3.12.0)
project(RTSPtoEcal VERSION 0.0.1)

set (CMAKE_CXX_STANDARD 14)
message(STATUS "CMAKE_SYSTEM_PROCESSOR is " ${CMAKE_SYSTEM_PROCESSOR})  #aarch64 for cross compiling, x86_64 for ubuntu
IF(NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})
    # cross compiling
    add_definitions(-DCROSS_COMPILING)  #for c++ macro
    SET(CROSS_COMPILING True)
ENDIF(NOT ${CMAKE_SYSTEM_PROCESSOR} MATCHES ${CMAKE_HOST_SYSTEM_PROCESSOR})


include(CTest)
enable_testing()
option(ON_MATRIX "open matirx" OFF)

find_package(eCAL REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

#libavfilter libswscale libpostproc
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)
IF(ON_MATRIX)
include_directories(${CMAKE_SOURCE_DIR}/matrix-dev-sdk/include)
link_directories(${CMAKE_SOURCE_DIR}/matrix-dev-sdk/lib)
ENDIF()

# the environmental variable of ON_MATRIX will be set by bitbake
set(ON_MATRIX $ENV{ON_MATRIX})
if (ON_MATRIX)
    message(STATUS  "open MATRIX")
    add_definitions(-DOPEN_MATRIX)  #for c++ macro
ENDIF()

add_subdirectory(Proto)
add_subdirectory(Helper)

AUX_SOURCE_DIRECTORY(./ RTSPTOECAL_src)

add_executable(${PROJECT_NAME} ${RTSPTOECAL_src})

IF (${NO_TYPE_DESCRIPTION})
    message(NOTICE  "No type or description for eCal")
    add_definitions(-DNO_TYPE_DESCRIPTION)  #for c++ macro
ELSE()
    message(NOTICE "Type or description for eCal")
ENDIF()

target_link_libraries(${PROJECT_NAME} eCAL::core)
target_link_libraries(${PROJECT_NAME} Protolib)
target_link_libraries(${PROJECT_NAME} Helper)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} mosquitto)

target_include_directories(${PROJECT_NAME} PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR} ${AVDEVICE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY}  ${SWSCALE_LIBRARY} ${AVDEVICE_LIBRARY})

IF (ON_MATRIX)
IF(${CROSS_COMPILING})
  message(STATUS "Will link arm .so file")
  target_link_libraries(${PROJECT_NAME} matrix-dev-sdk)
ELSE(${CROSS_COMPILING})
  message(STATUS "Will link x86 .so file")
  target_link_libraries(${PROJECT_NAME} matrix-dev-sdk_x86)
ENDIF(${CROSS_COMPILING})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/matrix-dev-sdk/lib/libmatrix-dev-sdk.so DESTINATION /usr/lib/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/matrixFilter.json DESTINATION /etc/${PROJECT_NAME}/)
ENDIF()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin")


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
