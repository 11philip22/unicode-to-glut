cmake_minimum_required(VERSION 3.10) 
project(AlchemicalSymbols)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL and GLUT
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Find FreeType
find_package(Freetype REQUIRED)

# Add executables
add_executable(AlchemicalSymbols AlchemicalSymbols.cpp)
add_executable(FontToGlut FontToGlut.cpp)

# Shared include directories
target_include_directories(AlchemicalSymbols PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
target_include_directories(FontToGlut PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

# Link libraries
target_link_libraries(AlchemicalSymbols ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FREETYPE_LIBRARIES})
target_link_libraries(FontToGlut ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FREETYPE_LIBRARIES})

# Optionally copy font file to build directory
file(COPY "Symbola.ttf" DESTINATION ${CMAKE_BINARY_DIR})