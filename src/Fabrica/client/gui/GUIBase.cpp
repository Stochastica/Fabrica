#include "GUIBase.hpp"

#include <cassert>

#include "../Window.hpp"
#include "../renderer/utils.hpp"

namespace fab
{

GLuint GUIBase::programPlanar;
GLuint GUIBase::programFlat;
GLuint GUIBase::programFlatPColor;
GLuint GUIBase::buffer;

void GUIBase::fillRect(int x0, int y0, int x1, int y1, std::uint32_t colour)
{
	x0 -= Window::instance().getWidth() / 2;
	x1 -= Window::instance().getWidth() / 2;
	y0 -= Window::instance().getHeight() / 2;
	y1 -= Window::instance().getHeight() / 2;
	float const c[] =  // Colour converted to float
	{
		(colour & 0xFF) / 255.f,
		(colour >> 8 & 0xFF) / 255.f,
		(colour >> 16 & 0xFF) / 255.f,
		(colour >> 24 & 0xFF) / 255.f
	};
	float const rw = Window::instance().getRatioW();
	float const rh = Window::instance().getRatioH();
	float const verts[] = // Vertex positions
	{
		x0 * rw, y0 * rh,
		x1 * rw, y0 * rh,
		x0 * rw, y1 * rh,
		x1 * rw, y1 * rh
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

	glUseProgram(programFlat);
	glUniform4f(programFlatPColor, c[0], c[1], c[2], c[3]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
}
void GUIBase::init()
{
	char const planarSourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec2 position;"
	  "layout(location = 1) in vec2 uvIn;"
	  "out vec2 uv;"
	  "void main()"
	  "{"
	  "gl_Position = vec4(position, 0, 1);"
	  "uv = uvIn;"
	  "}";
	char const planarSourceFrag[] =
	  "#version 330 core\n"
	  "in vec2 uv;"
	  "out vec4 color;"
	  "uniform sampler2D sampler;"
	  "void main()"
	  "{"
	  "color = texture(sampler, uv);"
	  "}";
	bool flag = registerGLProgram(&programPlanar,
	                              planarSourceVert,
	                              planarSourceFrag);
	assert(flag && "GL Program (Planar) compiling failed");
	(void) flag;
	char const flatSourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec2 position;"
	  "void main()"
	  "{"
	  "gl_Position = vec4(position, 0, 1);"
	  "}";
	char const flatSourceFrag[] =
	  "#version 330 core\n"
	  "out vec4 color;"
	  "uniform vec4 colorIn;"
	  "void main()"
	  "{"
	  "color = colorIn;"
	  "}";
	flag = registerGLProgram(&programFlat,
	                         flatSourceVert,
	                         flatSourceFrag);
	assert(flag && "GL Program (Flat) compiling failed");
	(void) flag;
	programFlatPColor = glGetUniformLocation(programFlat, "colorIn");

	glGenBuffers(1, &buffer);
}

} // namespace fab
