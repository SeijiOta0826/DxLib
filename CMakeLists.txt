cmake_minimum_required(VERSION 3.10)

project(DxLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(DXLIB_DIR "${CMAKE_SOURCE_DIR}/deps/include_dxlib")

include_directories(
    "${CMAKE_SOURCE_DIR}/Source"
    "${CMAKE_SOURCE_DIR}"
    "${DXLIB_DIR}"
)

file(GLOB_RECURSE SRC_FILES 
    "Source/*.cpp" 
    "Source/*.hpp"
    "main.cpp"
)

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

link_directories("${DXLIB_DIR}")

target_link_libraries(${PROJECT_NAME}
    "DxLib_x64.lib"
    "DxDirectX.lib"
    "DxUseCLib_x64.lib" 
    "libjpeg_static_x64.lib"
    "libpng_static_x64.lib"
    "zlib_static_x64.lib"
    "shlwapi.lib"
    "Vfw32.lib"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resource"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resource"
    COMMENT "Synchronizing assets to output directory..."
)