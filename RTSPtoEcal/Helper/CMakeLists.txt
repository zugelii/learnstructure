cmake_minimum_required(VERSION 3.12.2)
project(Helper VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

include(CTest)
enable_testing()
find_package(RapidJSON CONFIG REQUIRED) # RapidJSONConfig.cmake

set(SRC_FILE_LIST Helper.cpp Location.cpp module_alive.cpp
    Log.cpp
    GpsXForm.cpp)

add_library(Helper ${SRC_FILE_LIST})

target_include_directories(Helper PUBLIC "spdlog/include")

target_include_directories(Helper INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${RAPIDJSON_INCLUDE_DIRS}) # RapidJSON is self-contained and header-only
get_directory_property(parent PARENT_DIRECTORY)
if(parent)
    message(STATUS "Helper found. Headers: " ${CMAKE_CURRENT_LIST_DIR})
endif()

#windows/vc does not need to link explicitly
if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
