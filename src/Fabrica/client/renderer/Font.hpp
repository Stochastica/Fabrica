#ifndef FABRICA_CLIENT_RENDERER_FONT_HPP_
#define FABRICA_CLIENT_RENDERER_FONT_HPP_

#include <cassert>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace fab
{

#define FONT_NUMBER_GLYPHS 65536 // Must = planeWidth^2 * nPlanes
#define FONT_NUMBER_PLANES 256
#define FONT_PLANE_WIDTH 16
#define FONT_PLANE_SIZE 256 // Must = planeWidth^2

/**
 * @brief Provides an interface to FreeType for drawing texts on screen.
 * @warning Call {@code init()} before using this class.
 *
 * Unicode is not supported yet.
 *
 * {@code Font} is the only interface in Fabrica to FreeType.
 */
class Font final
{
public:
	/**
	 * @brief Initialise the FreeType library and text shaders.
	 * @return True if successful.
	 *
	 * This must be called after ModuleLoader is initialised. (After the
	 * default path is loaded in)
	 *
	 * This must preceed other calls to the {@code Font} class. If it
	 * fails, do not call any other functions in this class.
	 */
	static bool init();

	static Font const& defaultFont() { return defaultF; }

	Font();
	~Font();

	/**
	 * @brief Initialises the font. Must be called before other methods.
	 * @return True if successful.
	 */
	bool setFontFile(char const fileName[]);
	/**
	 * @brief Sets the font size and loads all glyphs.
	 * @warning Must be called after {@code setFontFile}. This function has a
	 *  high overhead.
	 *
	 * @return True if successful.
	 */
	bool setFontSize(unsigned int size);

	unsigned int getFontSize() const;

	/**
	 * @brief Loads via {@code glBindTexture} texture into memory.
	 */
	void loadTexture() const;

	/**
	 * @brief Get the width of a given character.
	 */
	float getCharWidth(int c) const noexcept;
	/**
	 * @brief Get the height of a given character.
	 */
	float getCharHeight(int c) const noexcept;
	/*
	 * @brief Get the advance X of a given character
	 */
	float getCharAdvX(int c) const noexcept;

	/**
	 * @brief Obtain the character information for a given char.
	 * @param[out] w The w coordinate of the character
	 * @param[out] u0 Left u coordinate
	 * @param[out] u1 Right u coordinate
	 * @param[out] v0 Top v coordinate
	 * @param[out] v1 Bottom v coordinate
	 * @param[out] ax Advance x
	 * @param[out] ay Advance y
	 * @param[out] sx Char width
	 * @param[out] sy Char height
	 * @param[out] mx Bitmap Left
	 * @param[out] my Bitmap Top
	 * @param[in] c The char
	 * @return True if the char occupies any volume.
	 */
	bool getChar(GLuint* const w,
	             float* const u0, float* const u1,
	             float* const v0, float* const v1,
	             float* const ax, float* const ay,
	             float* const sx, float* const sy,
	             float* const mx, float* const my,
	             int c) const noexcept;
private:
	static FT_Library ft;
	static Font defaultF;

	/**
	 * This struct stores the glyph data in pixels (not screen coordinates)
	 */
	struct CharData
	{
		float ax, ay; // Advance X, Advance Y
		float sx, sy; // Size X, Size Y
		float mx, my; // Min X, Min Y (bitmap_left, bitmap_top)
	} charData[FONT_NUMBER_GLYPHS];
	unsigned int size;
	/*
	 * Maximal size of a glyph
	 */
	int chartW, chartH;
	/*
	 * Width and Height of a plane of glyphs
	 * width = chartW * FONT_PLANE_WIDTH
	 * height = chartH * FONT_PLANE_WIDTH
	 */
	int width, height;

	FT_Face face;
	GLuint texture;
};

// Implementations

inline unsigned int Font::getFontSize() const
{
	return size;
}
inline void Font::loadTexture() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
}
inline float Font::getCharWidth(int c) const noexcept
{
	assert(0 <= c && c < FONT_NUMBER_GLYPHS &&
	       "Character Id exceeded limits");
	return charData[c].sx;
}
inline float Font::getCharHeight(int c) const noexcept
{
	assert(0 <= c && c < FONT_NUMBER_GLYPHS &&
	       "Character Id exceeded limits");
	return charData[c].sy;
}
inline float Font::getCharAdvX(int c) const noexcept
{
	assert(0 <= c && c < FONT_NUMBER_GLYPHS &&
	       "Character Id exceeded limits");
	return charData[c].ax;
}
inline bool Font::getChar(GLuint* const w,
                          float* const u0, float* const u1,
                          float* const v0, float* const v1,
                          float* const ax, float* const ay,
                          float* const sx, float* const sy,
                          float* const mx, float* const my,
                          int c) const noexcept
{
	assert(0 <= c && c < FONT_NUMBER_GLYPHS &&
	       "Character Id exceeded limits");

	auto const* const cd = &charData[c];

	*w = c / FONT_PLANE_SIZE;
	int planeId = c % FONT_PLANE_SIZE;

	int x = planeId % FONT_PLANE_WIDTH;
	int y = planeId / FONT_PLANE_WIDTH;

	float invWidth = 1.f / width;
	float invHeight = 1.f / height;
	*u0 = (x * chartW) * invWidth;
	*v0 = (y * chartH) * invHeight;
	*u1 = *u0 + cd->sx * invWidth;
	*v1 = *v0 + cd->sy * invHeight;
	*ax = cd->ax;
	*ay = cd->ay;
	*sx = cd->sx;
	*sy = cd->sy;
	*mx = cd->mx;
	*my = cd->my;

	return *ax != 0.f;
}

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_FONT_HPP_
