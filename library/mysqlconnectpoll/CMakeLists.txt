include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/lib/jsoncpp/include)

aux_source_directory(. SRC_LIST)

add_library(mysqlconnectpoll_shared SHARED ${SRC_LIST})

add_library(mysqlconnectpoll_static STATIC ${SRC_LIST})

set_target_properties(mysqlconnectpoll_shared PROPERTIES OUTPUT_NAME "mysqlconnectpoll")

set_target_properties(mysqlconnectpoll_static PROPERTIES OUTPUT_NAME "mysqlconnectpoll")

set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/lib)

