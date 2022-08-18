file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(Dependencies ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Math")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Audio")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/FastNoise")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/OpenGL")

set_target_properties(Dependencies PROPERTIES LINKER_LANGUAGE CXX)