cmake_minimum_required(VERSION 3.8)
project(ray_tracer)

set (CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

# adding the library needed by png++
add_library(png++ INTERFACE)
add_library(BSHOSHANY_THREAD_POOL INTERFACE)
target_include_directories(png++ INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/png++-0.2.9")
target_include_directories(BSHOSHANY_THREAD_POOL INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/thread-pool-3.5.0/include")
target_link_libraries(${PROJECT_NAME} PRIVATE png++ png BSHOSHANY_THREAD_POOL)

# for nice naming
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.exe")