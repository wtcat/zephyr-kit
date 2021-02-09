set(PROTO_GENERATOR protoc-c)
set(PROTO_PATH ${COMPONENTS_ROOT}/stp/proto)

add_custom_target(proto
    COMMAND
    ${PROTO_GENERATOR} --proto_path=${PROTO_PATH} --c_out=${PROTO_PATH} ${PROTO_PATH}/*.proto
)
