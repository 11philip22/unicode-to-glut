#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// Function to parse comma-separated hex codepoints
std::vector<unsigned int> parseCodepoints(const std::string& codepointStr) {
    std::vector<unsigned int> codepoints;
    std::stringstream ss(codepointStr);
    std::string item;
    while (std::getline(ss, item, ',')) {
        try {
            // Remove any leading/trailing whitespace
            item.erase(0, item.find_first_not_of(" \t"));\
            item.erase(item.find_last_not_of(" \t") + 1);
            // Convert hex string to unsigned int
            unsigned int codepoint = std::stoul(item, nullptr, 16);
            codepoints.push_back(codepoint);
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Invalid codepoint format: " << item << std::endl;
        }
    }
    return codepoints;
}

// Function to print usage information
void printUsage() {
    std::cerr << "Usage: ./generate_glyph_data -o <output_file> -c <codepoints> [-f <font_file>]\n";
    std::cerr << "  -o <output_file>: Output header file name (e.g., glyph_data.h)\n";
    std::cerr << "  -c <codepoints>: Comma-separated list of Unicode codepoints in hex (e.g., 1F714,1F70D,263F)\n";
    std::cerr << "  -f <font_file>: Font file path (default: Symbola.ttf)\n";
    std::cerr << "Example: ./generate_glyph_data -o glyph_data.h -c 1F714,1F70D,263F -f Symbola.ttf\n";
}

int main(int argc, char* argv[]) {
    // Default values
    std::string outputFile = "";
    std::string codepointStr = "";
    std::string fontPath = "Symbola.ttf";

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "-c" && i + 1 < argc) {
            codepointStr = argv[++i];
        } else if (arg == "-f" && i + 1 < argc) {
            fontPath = argv[++i];
        } else {
            printUsage();
            return 1;
        }
    }

    // Validate arguments
    if (outputFile.empty() || codepointStr.empty()) {
        printUsage();
        return 1;
    }

    // Parse codepoints
    std::vector<unsigned int> codepoints = parseCodepoints(codepointStr);
    if (codepoints.empty()) {
        std::cerr << "ERROR: No valid codepoints provided" << std::endl;
        printUsage();
        return 1;
    }

    // Initialize FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR: Could not init FreeType Library" << std::endl;
        return 1;
    }

    // Load font
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR: Failed to load font at " << fontPath << std::endl;
        FT_Done_FreeType(ft);
        return 1;
    }

    // Set font size
    FT_Set_Pixel_Sizes(face, 0, 48); // 48-pixel height, adjust as needed

    // Open output header file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Could not open " << outputFile << " for writing" << std::endl;
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return 1;
    }

    // Write header guard and initial comments
    std::string headerGuard = outputFile;
    std::transform(headerGuard.begin(), headerGuard.end(), headerGuard.begin(), ::toupper);
    std::replace(headerGuard.begin(), headerGuard.end(), '.', '_');
    outFile << "#ifndef " << headerGuard << "\n";
    outFile << "#define " << headerGuard << "\n\n";
    // outFile << "#include <ft2build.h>" << "\n";
    // outFile << "#include FT_FREETYPE_H" << "\n\n";
    outFile << "// Glyph texture data generated for specified Unicode symbols\n";
    outFile << "// Font: " << fontPath << ", Size: 48 pixels\n";
    outFile << "// Codepoints: " << codepointStr << "\n\n";

    // Process each codepoint
    for (unsigned int codepoint : codepoints) {
        if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER)) {
            std::cerr << "ERROR: Failed to load Glyph U+" << std::hex << codepoint << std::endl;
            continue;
        }

        // Access glyph bitmap and metrics
        FT_GlyphSlot slot = face->glyph;
        unsigned char* buffer = slot->bitmap.buffer;
        unsigned int width = slot->bitmap.width;
        unsigned int height = slot->bitmap.rows;
        unsigned int size = width * height;

        // Write glyph data array
        outFile << "static const unsigned char glyph_U" << std::hex << codepoint << "[] = {\n    ";
        for (unsigned int i = 0; i < size; ++i) {
            outFile << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
            if (i < size - 1) outFile << ", ";
            if ((i + 1) % 16 == 0 && i < size - 1) outFile << "\n    ";
        }
        outFile << "\n};\n";

        // Write glyph metrics
        outFile << std::dec; // Force decimal mode
        outFile << "static const int glyph_U" << std::hex << codepoint << "_width = " << std::dec << width << ";\n";
        outFile << "static const int glyph_U" << std::hex << codepoint << "_height = " << std::dec << height << ";\n";
        outFile << "static const int glyph_U" << std::hex << codepoint << "_bearingX = " << std::dec << slot->bitmap_left << ";\n";
        outFile << "static const int glyph_U" << std::hex << codepoint << "_bearingY = " << std::dec << slot->bitmap_top << ";\n";
        outFile << "static const int glyph_U" << std::hex << codepoint << "_advance = " << std::dec << (slot->advance.x >> 6) << ";\n\n";
    }

    // Close header guard
    outFile << "#endif // " << headerGuard << "\n";

    // Clean up
    outFile.close();
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::cout << "Generated " << outputFile << " successfully" << std::endl;
    return 0;
}