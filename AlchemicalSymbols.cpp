#include <GL/glut.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <vector>
#include <map>

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

// Map to store glyph textures for each Unicode code point
std::map<unsigned int, GlyphTexture> glyphs;

// Symbol pairs with Unicode code points
struct SymbolPair {
    unsigned int first;
    unsigned int second;
    std::string label; // For display (e.g., "Salt, Mercury")
};

std::vector<SymbolPair> pairs = {
    {0x1F714, 0x1F714, "🜔, 🜔"}, // Salt, Salt
    {0x1F714, 0x263F,  "🜔, ☿"},  // Salt, Mercury
    {0x1F714, 0x1F70D, "🜔, 🜍"}, // Salt, Sulfur
    {0x263F,  0x1F714, "☿, 🜔"},  // Mercury, Salt
    {0x263F,  0x263F,  "☿, ☿"},   // Mercury, Mercury
    {0x263F,  0x1F70D, "☿, 🜍"},  // Mercury, Sulfur
    {0x1F70D, 0x1F714, "🜍, 🜔"}, // Sulfur, Salt
    {0x1F70D, 0x263F,  "🜍, ☿"},  // Sulfur, Mercury
    {0x1F70D, 0x1F70D, "🜍, 🜍"}  // Sulfur, Sulfur
};

// Initialize FreeType and load glyphs
bool initFreeType(FT_Library& ft, FT_Face& face) {
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR: Could not init FreeType Library" << std::endl;
        return false;
    }

    // Specify the path to your font file
    const char* fontPath = "Symbola.ttf"; // Adjust path as needed
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "ERROR: Failed to load font" << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 48); // Set font size

    // Load glyphs for required symbols
    std::vector<unsigned int> codepoints = {0x1F714, 0x1F70D, 0x263F};
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for (unsigned int codepoint : codepoints) {
        if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
            std::cerr << "ERROR: Failed to load Glyph U+" << std::hex << codepoint << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store glyph info
        GlyphTexture glyph = {
            texture,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<int>(face->glyph->advance.x >> 6)
        };
        glyphs[codepoint] = glyph;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

// Render a single glyph at position (x, y)
void renderGlyph(unsigned int codepoint, float x, float y) {
    if (glyphs.find(codepoint) == glyphs.end()) return;

    GlyphTexture& g = glyphs[codepoint];

    float xpos = x + g.bearingX;
    float ypos = y - (g.height - g.bearingY);

    float w = g.width;
    float h = g.height;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g.textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos + h);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos + h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable blending for texture transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render symbol pairs
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    float startX = 50.0f;
    float startY = WINDOW_HEIGHT - 50.0f;
    float lineSpacing = 60.0f;
    float symbolSpacing = 50.0f;

    for (size_t i = 0; i < pairs.size(); ++i) {
        float y = startY - i * lineSpacing;
        float x = startX;

        // Render first symbol
        renderGlyph(pairs[i].first, x, y);
        x += symbolSpacing;

        // Render comma (using GLUT bitmap font as placeholder)
        glRasterPos2f(x, y + 15.0f);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ',');
        x += 20.0f;

        // Render second symbol
        renderGlyph(pairs[i].second, x, y);
    }

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    // Initialize FreeType
    FT_Library ft;
    FT_Face face;
    if (!initFreeType(ft, face)) {
        std::cerr << "Failed to initialize FreeType" << std::endl;
        exit(1);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Alchemical Symbols");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}