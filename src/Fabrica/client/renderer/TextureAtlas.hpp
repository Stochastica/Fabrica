#ifndef FABRICA_CLIENT_RENDERER_TEXTUREATLAS_HPP_
#define FABRICA_CLIENT_RENDERER_TEXTUREATLAS_HPP_

#include <GL/glew.h>

namespace fab
{

/**
 * @brief Fixed-dimension Texture Atlas
 *
 * Internal parameters:
 * Mipmap level = 1
 * Format = GL_RGBA8 (RGB + Alpha, 8bits/chan)
 */
class TextureAtlas final
{
public:
	/**
	 * @brief Creates a texture atlas object.
	 *
	 * Each texture in the atlas is a "chart". Internally,
	 * {@code TextureAtlas} uses an OpenGL 2D array texture to store
	 * the textures. Each layer of the 2D array is a "plane".
	 *
	 * @param[in] chartW Width of each chart
	 * @param[in] chartH Height of each chart
	 * @param[in] planeW Number (Horizontal) of charts in each plane.
	 * @param[in] planeH Number (Vertical) of charts in each plane
	 * @param[in] depth Number of planes
	 *
	 */
	TextureAtlas(int chartW, int chartH,
	             int planeW, int planeH,
	             int depth);
	/**
	 * Deletes the internal GL texture.
	 */
	~TextureAtlas();

	/**
	 * @brief Loads texture into memory.
	 * @warning Not responsible for activating texture.
	 */
	void loadTexture();
	/**
	 * @brief Loads the pixels into the given chart
	 *
	 * Must call {@code loadTexture()} before executing this. No
	 * texture loading should be done between {@code loadTexture()} and this
	 * function.
	 *
	 * @param[in] chartId The id of the chart.
	 * @param[in] pixels Pixels
	 */
	void loadChart(int chartId, void* pixels);


	/**
	 * @brief Returns the UVW coordinates for a chart.
	 *
	 * @param[out] w W coordinate
	 * @param[out] u0 Left U
	 * @param[out] u1 Right U
	 * @param[out] v0 Top V
	 * @param[out] v1 Bottom V
	 * @param[in] chartId
	 */
	void chartUVW(GLuint* const w,
	              float* const u0, float* const u1,
	              float* const v0, float* const v1,
	              int chartId) const noexcept;

private:
	GLuint texture;

	int chartW, chartH; // Width/Height of a chart
	int planeW, planeH; // Number of charts in a plane,
	// Horizontally and Vertically
	int nChartsInPlane; // Always = planeW * planeH
	/*
	 * width = chartW * planeW
	 * height = chartH * planeH
	 * depth = Number of planes
	 */
	int width, height, depth;
#ifndef NDEBUG
	int maxCharts;
#endif
};

// Implementations

inline void TextureAtlas::loadTexture()
{
	// glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
}

inline void TextureAtlas::chartUVW(GLuint* const w,
                                   float* const u0, float* const u1,
                                   float* const v0, float* const v1,
                                   int chartId) const noexcept
{
	*w = chartId / nChartsInPlane;
	int planeId = chartId % nChartsInPlane;

	int x = planeId % planeW;
	int y = planeId / planeW;

	*u0 = (x * chartW) / (float) width;
	*v0 = (y * chartH) / (float) height;
	*u1 = ((x + 1) * chartW - 1) / (float) width;
	*v1 = ((y + 1) * chartH - 1) / (float) height;
}

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_TEXTUREATLAS_HPP_
