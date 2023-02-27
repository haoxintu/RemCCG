cmake_minimum_required(VERSION 3.5)

set(FUZZING_FLAGS "")
set(FUZZING_FLAGS_BINARY "")
set(FUZZING_FLAGS_FUZZER "-fsanitize-coverage=edge,trace-pc-guard,indirect-calls,trace-cmp,trace-div,trace-gep -fsanitize=fuzzer")
include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

protobuf_generate_cpp(C_PROTO_SRCS C_PROTO_HDRS c_proto.proto)

add_library(c-proto ${C_PROTO_SRCS})
add_library(proto_to_c
    proto_to_c.cc
    util.cc
)

target_link_libraries(proto_to_c
    c-proto
    ccg
)

add_executable(remccg
    remccg.cc
    util.cc
)

target_link_libraries(remccg
    c-proto
    proto_to_c
    ${PROTOBUF_LIBRARIES}
    ccg
)

set_property(TARGET remccg
    PROPERTY COMPILE_FLAGS ${FUZZING_FLAGS}
)

set_property(TARGET remccg
    PROPERTY LINK_FLAGS ${FUZZING_FLAGS_BINARY}
)

install (TARGETS remccg RUNTIME DESTINATION bin)
