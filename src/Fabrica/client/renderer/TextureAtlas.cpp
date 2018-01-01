#include "TextureAtlas.hpp"

#include <cassert>

#include "utils.hpp"

namespace fab
{

TextureAtlas::TextureAtlas(int chartW, int chartH,
                             int planeW, int planeH,
                             int depth):
	chartW(chartW), chartH(chartH),
	planeW(planeW), planeH(planeH),
	nChartsInPlane(planeW * planeH),
	width(chartW * planeW), height(chartH * planeH), depth(depth)
#ifndef NDEBUG
	,maxCharts(width * height * depth)
#endif
{
#ifndef NDEBUG
	{
		assert(width > 0 && "Zero width is not permitted");
		assert(height > 0 && "Zero height is not permitted");
		assert(depth > 0 && "Zero depth is not permitted");
		// Check if parameters are permitted
		GLint maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		GLint maxArrayTextureLayers;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayTextureLayers);

		assert(width <= maxTextureSize&&
		       "chartW * planeW exceeded OpenGL limit");
		assert(height <= maxTextureSize&&
		       "chartH * planeH exceeded OpenGL limit");
		assert(depth <= maxArrayTextureLayers&&
		       "depth exceeded OpenGL limit");
	}
#endif
	// Active and generate the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	// Sets the alignment.
	// In our case, the default alignment of 4 suffices.
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*
	 * Generates the texture storage
	 *
	 * Warning: The mipmap level makes some assumptions on the
	 * minimal size of chartW * planeW, etc.
	 */
	glTextureStorage3D(texture, // Texture
	                   1, // Mipmap level = 1
	                   GL_RGBA8, // Format
	                   width, height, depth // Dimensions
	                   );
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GL_ERROR_CHECK;
}
TextureAtlas::~TextureAtlas()
{
	glDeleteTextures(1, &texture);
}

void TextureAtlas::loadChart(int chartId, void* pixels)
{
#ifndef NDEBUG
	assert(chartId < maxCharts &&
			"chartId exceeded limit");
#endif
	int x, y, z;

	{ // Break chartId down into components
		z = chartId / nChartsInPlane;
		int planeId = chartId % nChartsInPlane;
		y = planeId / planeW;
		x = planeId % planeW;
	}

	glTextureSubImage3D(texture, // Target
			0, // Mipmap level
			x * chartW, // Offset X
			y * chartH, // Offset Y
			z,            // Offset Z
			chartW,       // Size X
			chartH,       // Size Y
			1,            // Size Z
			GL_RGBA,     // Type
			GL_UNSIGNED_BYTE, // Input type
			pixels);

	GL_ERROR_CHECK;
}

} // namespace fab
