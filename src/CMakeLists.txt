include_directories(${PROJECT_SOURCE_DIR}/include} ${PROJECT_SOURCE_DIR}/lib/jsoncpp/include)

aux_source_directory(. SRC_LIST)

add_executable(main ${SRC_LIST} ../include/Configuration.h Configuration.cpp ../include/Configuration.h)

find_library(MYSQLCONNECTPOLL mysqlconnectpoll HINTS ${PROJECT_SOURCE_DIR}/lib)

find_library(jsoncpp jsoncpp HINTS ${PROJECT_SOURCE_DIR}/lib/jsoncpp/lib)

target_link_libraries(main ${MYSQLCONNECTPOLL} mysqlclient ${jsoncpp} pthread)

add_compile_options(-std=C++11 -Wall)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)