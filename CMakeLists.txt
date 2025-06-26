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
add_executable(FontToGlyph FontToGlyph.cpp)
add_executable(EmbeddedAlchemicalSymbols EmbeddedAlchemicalSymbols.cpp AlchemicalGlyphs.h)

# Shared include directories
target_include_directories(AlchemicalSymbols PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
target_include_directories(FontToGlyph PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
target_include_directories(EmbeddedAlchemicalSymbols PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})

# Link libraries
target_link_libraries(AlchemicalSymbols ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FREETYPE_LIBRARIES})
target_link_libraries(FontToGlyph ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FREETYPE_LIBRARIES})
target_link_libraries(EmbeddedAlchemicalSymbols ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${FREETYPE_LIBRARIES})

# Copy font file to build directory if AlchemicalSymbols is build
add_custom_target(copy_symbola_font
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/Symbola.ttf"
    "${CMAKE_BINARY_DIR}/Symbola.ttf"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Symbola.ttf"
    COMMENT "Copying Symbola.ttf to build directory"
)
add_dependencies(AlchemicalSymbols copy_symbola_font)