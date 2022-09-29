set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

function(initDeps)
    file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/*.h")
    file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/*.cpp")

    add_library(Dependencies ${HEADER_FILES} ${SOURCE_FILES})

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Math)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Audio)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FastNoise)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/OpenGL)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/UI)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Network)
    if(WIN32)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/include)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include)
    else()
        find_package(Freetype REQUIRED)
        include_directories(${FREETYPE_INCLUDE_DIRS})
    endif()
endfunction()

function(afterExec)
    target_link_libraries(${PROJECT_NAME} PRIVATE Dependencies)
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/libglfw3.a
                ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/freetype.a
                wsock32
                ws2_32)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE
                glfw
                Freetype::Freetype
                -lpthread)
    endif()
endfunction()