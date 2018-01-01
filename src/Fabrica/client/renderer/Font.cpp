#include "Font.hpp"

#include <algorithm>
#include <iostream>

#include "utils.hpp"
#include "../../util/ResourceLocation.hpp"
#include "../../common/ModuleLoader.hpp"

namespace fab
{

int const defaultFontSize = 20;

FT_Library Font::ft;
Font Font::defaultF;

bool Font::init()
{
	if (FT_Init_FreeType(&ft))
	{
		assert(false && "class Font: FT library Init failed");
		return false;
	}

	ResourceLocation fontLocation("fabrica", "font/DejaVuSansMono.ttf");
	auto defaultPath = ModuleLoader::instance().resolveLocation(fontLocation);

	if (!defaultF.setFontFile(defaultPath.c_str()))
	{
		assert(false && "class Font: Failed to load default Font");
		return false;
	}
	defaultF.setFontSize(20);

	return true;
}

Font::Font():
	size(0),
	width(0),
	height(0),
	texture(0)
{
	// Must not call any GL function here
	// The default font is being initialised.
}
Font::~Font()
{
	glDeleteTextures(1, &texture);
	FT_Done_Face(face);
}
bool Font::setFontFile(char const fileName[])
{
	if (FT_New_Face(ft, fileName, 0, &face))
		return false;
	return true;
}
bool Font::setFontSize(unsigned int size)
{
	glDeleteTextures(1, &texture);
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	assert(size != 0);
	this->size = size;

	// FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	FT_Set_Pixel_Sizes(face, 0, size);

	// First loop: Calculate maxW and maxH
	chartW = 0;
	chartH = 0;
	for (int i = 0; i < FONT_NUMBER_GLYPHS ; ++i)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			continue;
		chartW = std::max<int>(chartW, face->glyph->bitmap.width);
		chartH = std::max<int>(chartH, face->glyph->bitmap.rows);
	}
	if (chartW == 0 || chartH == 0)
	{
		std::cerr << "No Valid Glyph found\n";
		return false;
	}

	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	width = chartW * FONT_PLANE_WIDTH;
	height = chartH * FONT_PLANE_WIDTH;

	// 8bit/pixel, so alignment = 1
	glTextureStorage3D(texture,
	                   1,
	                   GL_R8, // Single channel format
	                   width,
	                   height,
	                   FONT_NUMBER_PLANES
	                  );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GL_ERROR_CHECK;

	// Load Glyphs

	for (int plane = 0; plane < FONT_NUMBER_PLANES; ++plane)
	{
		for (int i = 0; i < FONT_PLANE_WIDTH; ++i) // Row
		{
			for (int j = 0; j < FONT_PLANE_WIDTH; ++j) // Column
			{
				// Char id = rowShift + j

				int id = plane * FONT_PLANE_SIZE
				         + i * FONT_PLANE_WIDTH
				         + j;
				auto* cd = &charData[id];
				if (FT_Load_Char(face, id, FT_LOAD_RENDER))
				{
					// Loading failed
					cd->ax = 0.f;
					cd->ay = 0.f;
					cd->sx = 0.f;
					cd->sy = 0.f;
					cd->mx = 0.f;
					cd->my = 0.f;
				}
				else
				{
					glTextureSubImage3D(texture, // Texture
					                    0, // Mipmap level
					                    j * chartW, // Offset X
					                    i * chartH, // Offset Y
					                    plane,      // Offset Z
					                    face->glyph->bitmap.width, // Size X
					                    face->glyph->bitmap.rows,  // Size Y
					                    1,                         // Size Z
					                    GL_RED,                    // Type
					                    GL_UNSIGNED_BYTE,          // Len.
					                    face->glyph->bitmap.buffer // Pixels
					                   );

					// The advance.[x,y] in Freetype are given in 1/64 pixels.
					cd->ax = face->glyph->advance.x / 64.f;
					cd->ay = face->glyph->advance.y / 64.f;
					cd->sx = face->glyph->bitmap.width;
					cd->sy = face->glyph->bitmap.rows;
					cd->mx = face->glyph->bitmap_left;
					cd->my = face->glyph->bitmap_top;
				}
			}
		}
	}
	GL_ERROR_CHECK;

	return true;
}

} // namespace fab
