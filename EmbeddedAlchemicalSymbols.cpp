#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <map>

// Include the generated glyph data header
#include "AlchemicalGlyphs.h"

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Structure to hold texture info for a glyph
struct GlyphTexture {
    GLuint textureID;
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advance;
};

// Map to store glyph textures
std::map<unsigned int, GlyphTexture> glyphs;

// Function to create OpenGL texture from static glyph data
GLuint createTextureFromGlyphData(const unsigned char* data, int width, int height) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

// Initialize textures from static glyph data
bool initGlyphs() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Salt symbol (U+1F714)
    glyphs[0x1F714] = {
        createTextureFromGlyphData(glyph_U1f714, glyph_U1f714_width, glyph_U1f714_height),
        glyph_U1f714_width, glyph_U1f714_height,
        glyph_U1f714_bearingX, glyph_U1f714_bearingY, glyph_U1f714_advance
    };

    // Sulfur symbol (U+1F70D)
    glyphs[0x1F70D] = {
        createTextureFromGlyphData(glyph_U1f70d, glyph_U1f70d_width, glyph_U1f70d_height),
        glyph_U1f70d_width, glyph_U1f70d_height,
        glyph_U1f70d_bearingX, glyph_U1f70d_bearingY, glyph_U1f70d_advance
    };

    // Mercury symbol (U+263F)
    glyphs[0x263F] = {
        createTextureFromGlyphData(glyph_U263f, glyph_U263f_width, glyph_U263f_height),
        glyph_U263f_width, glyph_U263f_height,
        glyph_U263f_bearingX, glyph_U263f_bearingY, glyph_U263f_advance
    };

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

// Render a glyph at position (x, y)
void renderGlyph(unsigned int codepoint, float x, float y) {
    if (glyphs.find(codepoint) == glyphs.end()) return;

    GlyphTexture& g = glyphs[codepoint];
    float xpos = x + g.bearingX;
    float ypos = y - (g.height - g.bearingY);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g.textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + g.width, ypos);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + g.width, ypos + g.height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos + g.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Simply render the three alchemical symbols
    renderGlyph(0x1F714, 100, 400); // Salt
    renderGlyph(0x1F70D, 250, 400); // Sulfur  
    renderGlyph(0x263F, 400, 400);  // Mercury

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        for (auto& pair : glyphs) {
            glDeleteTextures(1, &pair.second.textureID);
        }
        exit(0);
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    if (!initGlyphs()) {
        std::cerr << "Failed to initialize glyphs" << std::endl;
        exit(1);
    }
    
    std::cout << "Glyphs loaded. Press ESC to exit." << std::endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Simple Glyph Renderer");
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    init();
    glutMainLoop();
    return 0;
}