#ifndef FABRICA_CLIENT_RENDERER_TEXT_HPP_
#define FABRICA_CLIENT_RENDERER_TEXT_HPP_

#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "Font.hpp"
#include "../../common/fabrica.hpp"
#include "../../util/vector.hpp"

namespace fab
{

/**
 * @brief Handles text drawing with OpenGL
 * @warning
 *  Call {@code init()} before using this class. An OpenGL context must be
 *  present when drawing.
 *
 * This class differs from Font since it bridges between Font and Window.
 *
 * TODO: Remove all the GL_ERROR_CHECKs
 */
class Text final
{
public:
	/**
	 * @brief Initialises the shaders.
	 * @warning
	 *  Must be called after loading OpenGL context and before any other
	 *  invocation to this class.
	 *
	 * This function asserts that the shaders will be compiled correctly.
	 */
	static void init();
	/*
	 * Describe how is the position of the text relative to the set position of
	 * the text.
	 */
	enum Align: std::int8_t
	{
		BOT_LEFT = 0,
		BOT_RIGHT,
		TOP_LEFT,
		TOP_RIGHT
	};

	Text() noexcept;
	Text(Font const* const font) noexcept;
	~Text();

	void draw() const; // Draw with color=white.
	void draw(float color[]) const;

	void setContents(float x, float y, Align align, String str) noexcept;

private:
	/**
	 * @brief Loads a line of text into the geometry.
	 * @return True if any geometry is added.
	 */
	bool loadLine(std::vector<Vector2f>* vertices,
	              std::vector<Vector2f>* uv,
	              std::vector<GLuint>* w,
	              float x, float y, Align align,
	              String str,
	              float const ratioW, float const ratioH) const;

	/**
	 * Default 2D text drawing shader.
	 */
	static GLuint program;
	/**
	 * Color parameter in {@code program}.
	 */
	static GLuint programPColor;
	static GLuint buffer; ///< GL buffer for text rectangle

	Font const* font;
	Align align;

	// If false, the drawing routine is skipped.
	bool hasGeometry;

	/*
	 * Buffers for drawing textures
	 */
	GLuint bufferVert, bufferUV, bufferLayers, bufferIndices;
	unsigned int nIndices;
};

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_TEXT_HPP_
