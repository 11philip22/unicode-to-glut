cmake_minimum_required(VERSION 3.10) 
project(AlchemicalSymbols)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Find OpenGL
find_package(OpenGL REQUIRED)

#Find GLUT
find_package(GLUT REQUIRED)

#Find FreeType
find_package(Freetype REQUIRED)

#Include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

#Add executable
add_executable(AlchemicalSymbols main.cpp)

#Link libraries
target_link_libraries(AlchemicalSymbols ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FREETYPE_LIBRARIES})

#Optionally copy font file to build directory
file(COPY "Symbola.ttf" DESTINATION ${CMAKE_BINARY_DIR})
