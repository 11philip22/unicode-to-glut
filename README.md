
# Unicode to Glut

Render unicode symbols in C++ using OpenGL and FreeType.

## Apps
- **AlchemicalSymbols**: Loads glyphs from `Symbola.ttf` at runtime (needs FreeType).
- **EmbeddedAlchemicalSymbols**: Uses pre-generated glyphs from `AlchemicalGlyphs.h` (no FreeType needed).
- **FontToGlyph**: Generates `AlchemicalGlyphs.h` from a font and Unicode codepoints.

## Build & Run
1. Create a build directory:
  ```bash
  mkdir build && cd build
  cmake ..
  cmake --build .
  ```
2. Run apps:
  ```bash
  ./AlchemicalSymbols
  ./EmbeddedAlchemicalSymbols
  ./FontToGlyph -o alchemical_glyphs.h -c 1F714,1F70D,263F -f ../Symbola.ttf
  ```

## Requirements
- CMake ≥ 3.10
- C++17 compiler
- OpenGL, GLUT
- FreeType (for AlchemicalSymbols, FontToGlyph)
- `Symbola.ttf` font

## Notes
- `Symbola.ttf` must support the required Unicode symbols.
- Run `FontToGlyph` to regenerate glyphs for different codepoints/fonts.
- Default font size: 48px. Default window: 800x600.

## License
Educational use only. Check font license for redistribution.