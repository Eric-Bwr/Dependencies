file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.c" "*.cpp")

add_library(Dependencies ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Math")

set_target_properties(Dependencies PROPERTIES LINKER_LANGUAGE CXX)