#ifndef FABRICA_CLIENT_GUI_GUIBASE_HPP_
#define FABRICA_CLIENT_GUI_GUIBASE_HPP_

#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace fab
{

/**
 * @brief Base of all GUI elements and layers.
 *
 * A GUI is a graphics that can be displayed onto the screen in 2D. e.g. an
 * opened inventory, HUD, the loading screen, main menu, a button, etc.
 */
class GUIBase
{
public:
	virtual ~GUIBase() = default;

	/**
	 * @brief Initialises GLSL shaders.
	 * @warning Must be called after creation of OpenGL context
	 */
	static void init();
//protected:
	/**
	 * @brief Fills a rectangle
	 * @param[in] x0 Left bound
	 * @param[in] y0 Up bound
	 * @param[in] x1 Right bound
	 * @param[in] y1 Bottom bound
	 * @param[in] color Color in 0xAABBGGRR format
	 */
	static void fillRect(int x0, int y0, int x1, int y1, std::uint32_t color);

private:
	static GLuint programPlanar; // Program for drawing textured 2d objects
	static GLuint programFlat; // Program for drawing flat colors
	static GLuint programFlatPColor; // Program uniform parameter: color
	static GLuint buffer; // Buffer for drawing rects
};

} // namespace fab

#endif // !FABRICA_CLIENT_GUI_GUIBASE_HPP_
