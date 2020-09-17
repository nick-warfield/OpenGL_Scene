cmake_minimum_required(VERSION 3.2)
project(OpenGL_Scene)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/include/*.h
	${CMAKE_SOURCE_DIR}/include/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
find_package(OpenGL REQUIRED)

# GLM
find_package(glm REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${glfw3_DIR}")

# ASSIMP
find_package(assimp REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# STB_IMAGE
add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/src/stb_image.cpp")

# GLAD
add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")

# Put all libraries into a variable
set(LIBS glfw GL X11 pthread Xrandr Xi dl)
include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} ${LIBS})
