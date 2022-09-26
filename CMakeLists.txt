file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(Dependencies ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Math")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Audio")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/FastNoise")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/OpenGL")
target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/UI")
if(WIN32)
    target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/FreeType/include")
    target_include_directories(Dependencies PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/GLFW/include")
endif()

function(linkDeps)
    target_link_libraries(${PROJECT_NAME} PRIVATE Dependencies)

    if(WIN32)
        target_link_libraries(${PROJECT_NAME} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/libglfw3.a
                ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/freetype.a)
    else()
        target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE glfw Freetype::Freetype)
    endif()
endfunction()