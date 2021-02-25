cmake_minimum_required(VERSION 3.12.0)
project(Protolib VERSION 0.1.0)

Find_Package(Protobuf REQUIRED)

include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

create_targets_protobuf()

set(proto_proto
  ${CMAKE_CURRENT_SOURCE_DIR}/type.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/rsm.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/rsi.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/heartbeat.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/comm.proto
)
#AUX_SOURCE_DIRECTORY(./ proto_proto)
add_library(${PROJECT_NAME} "")

protobuf_target_cpp(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} ${proto_proto})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROTOBUF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROTOBUF_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
