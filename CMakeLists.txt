cmake_minimum_required(VERSION 3.10)
project(ZingPAD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Directorios
include_directories(include)

# OPENCV VCPGK
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
# ARCHIVOS FUENTE
file(GLOB_RECURSE SOURCES "src/*.cpp")

# EJECUTABLE
add_executable(ZingPAD ${SOURCES})

# VINCUALACION
target_link_libraries(ZingPAD ${OpenCV_LIBS})
