#include "Text.hpp"

#include <cassert>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "../Window.hpp"
#include "../renderer/utils.hpp"

namespace fab
{

GLuint Text::program;
GLuint Text::programPColor;
GLuint Text::buffer;

void Text::init()
{
	char const sourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec2 vertPosition;"
	  "layout(location = 1) in vec2 vertexUV;"
	  "layout(location = 2) in uint tIdIn;"
	  "out vec2 uv;"
	  "flat out uint tId;"
	  "void main()"
	  "{"
	  "gl_Position = vec4(vertPosition, 0, 1);"
	  "uv = vertexUV;"
	  "tId = tIdIn;"
	  "}";
	char const sourceFrag[] =
	  "#version 330 core\n"
	  "in vec2 uv;"
	  "flat in uint tId;"
	  "out vec4 color;"
	  "uniform sampler2DArray sampler;"
	  "uniform vec4 textColor;"
	  "void main()"
	  "{"
	  "color = vec4(1,1,1,texture(sampler, vec3(uv, tId)).r) *"
	  "        textColor;"
	  "}";
	bool flag = registerGLProgram(&program, sourceVert, sourceFrag);
	assert(flag && "class Text: GL Program register failed");
	(void) flag;
	programPColor = glGetUniformLocation(program, "textColor");
}

Text::Text() noexcept:
		font(nullptr),
		hasGeometry(false)
{
	glGenBuffers(1, &bufferVert);
	glGenBuffers(1, &bufferUV);
	glGenBuffers(1, &bufferLayers);
	glGenBuffers(1, &bufferIndices);
}
Text::Text(Font const* const font) noexcept:
		font(font),
		align(BOT_LEFT),
		hasGeometry(false)
{
	glGenBuffers(1, &bufferVert);
	glGenBuffers(1, &bufferUV);
	glGenBuffers(1, &bufferLayers);
	glGenBuffers(1, &bufferIndices);
}
Text::~Text()
{
	glDeleteBuffers(1, &bufferVert);
	glDeleteBuffers(1, &bufferUV);
	glDeleteBuffers(1, &bufferLayers);
	glDeleteBuffers(1, &bufferIndices);
}

void Text::draw() const
{
	static float colorWhite[] = {1.f, 1.f, 1.f, 1.f};
	draw(colorWhite);
}
void Text::draw(float color[]) const
{
	if (!hasGeometry) return;
	assert(font);

	glUseProgram(program);
	glUniform4f(programPColor,
	            color[0],
	            color[1],
	            color[2],
	            color[3]);

	glActiveTexture(GL_TEXTURE0);
	font->loadTexture();

	glEnableVertexAttribArray(0); // Position array
	glEnableVertexAttribArray(1); // UV array
	glEnableVertexAttribArray(2); // Texture id array

	glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, bufferLayers);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Text::setContents(float x, float y, Align align, String str) noexcept
{
	if (str.empty())
	{
		hasGeometry = false;
		return; // No geometry needed
	}
	// Initial Character X and Y
	float charX = x;
	float charY = y;
	float ratioW = Window::instance().getRatioW();
	float ratioH = Window::instance().getRatioH();

	// charX and charY should always be located at the bottom-left of
	// each character.
	// Right alignment requires shifting the X coordinate
	// Top aligment requires shifting the Y coordinate
	if (align == TOP_LEFT || align == TOP_RIGHT)
	{
		/*
		float height = font->getCharHeight(align == TOP_RIGHT ?
		                                   str.back() :
		                                   str.front());
		charY -= height * ratioH;
		*/
		charY -= font->getFontSize() * ratioH;
	}

	// Split string into lines
	std::vector<String> lines;
	boost::split(lines, str, boost::is_any_of("\n"));

	// Calculate number of chars
	int nChar = 0;
	for (auto const& l : lines)
	{
		nChar += l.size();
	}
	if (nChar == 0)
	{
		hasGeometry = false;
		return;
	}

	std::vector<Vector2f> vertices;
	std::vector<Vector2f> uv;
	std::vector<GLuint> layers;
	vertices.reserve(nChar * 4);
	uv.reserve(nChar * 4);
	layers.reserve(nChar * 4);

	switch (align)
	{
	case TOP_LEFT:
	case TOP_RIGHT:
		for (auto const& l: lines)
		{
			if (align == TOP_RIGHT)
			{
				charX = x - font->getCharAdvX(str.back()) * ratioW;
			}
			hasGeometry = loadLine(&vertices, &uv, &layers,
			                       charX, charY, align,
			                       l, ratioW, ratioH) ||
			              hasGeometry;
			charY -= font->getFontSize() * ratioH;
		}
		break;
	case BOT_LEFT:
	case BOT_RIGHT:
		std::reverse(lines.begin(), lines.end());
		for (auto const& l: lines)
		{
			if (align == BOT_RIGHT)
			{
				charX = x - font->getCharAdvX(str.back()) * ratioW;
			}
			hasGeometry = loadLine(&vertices, &uv, &layers,
			                       charX, charY, align,
			                       l, ratioW, ratioH) ||
			              hasGeometry;
			charY += font->getFontSize() * ratioH;
		}
		break;
	default:
		assert(false && "class Text: Unhandled align case");
	}

	// Load into GL
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
		glBufferData(GL_ARRAY_BUFFER,
		             vertices.size() * sizeof(Vector2f),
		             vertices.data(),
		             GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
		glBufferData(GL_ARRAY_BUFFER,
		             uv.size() * sizeof(Vector2f),
		             uv.data(),
		             GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, bufferLayers);
		glBufferData(GL_ARRAY_BUFFER,
		             layers.size() * sizeof(GLuint),
		             layers.data(),
		             GL_DYNAMIC_DRAW);
		nIndices = generateQuadElementIndices(&bufferIndices, vertices.size());
	}
}

bool Text::loadLine(std::vector<Vector2f>* vertices,
                    std::vector<Vector2f>* uv,
                    std::vector<GLuint>* layers,
                    float charX, float charY, Align align,
                    String str,
                    float const ratioW, float const ratioH) const
{
	if (str.empty()) return false;


	bool flag = false; // Does this line have geometry?

	switch (align)
	{
	case BOT_LEFT:
	case TOP_LEFT:
		for (auto const& c : str)
		{
			// Texture parameters
			GLuint w;
			float u0, u1, v0, v1;
			// Glyph parameters
			float ax, ay, sx, sy, mx, my;

			bool hasVolume = font->getChar(&w,
			                               &u0, &u1,
			                               &v0, &v1,
			                               &ax, &ay,
			                               &sx, &sy,
			                               &mx, &my,
			                               c);
			if (!hasVolume) continue;

			flag = true;
			// Insert Geometry into graphics

			float x2 = charX + mx * ratioW;
			float y2 = charY + my * ratioH;
			float charW = sx * ratioW;
			float charH = sy * ratioH;

			vertices->push_back(Vector2f(x2,         y2));
			vertices->push_back(Vector2f(x2,         y2 - charH));
			vertices->push_back(Vector2f(x2 + charW, y2 - charH));
			vertices->push_back(Vector2f(x2 + charW, y2));

			uv->push_back(Vector2f(u0, v0));
			uv->push_back(Vector2f(u0, v1));
			uv->push_back(Vector2f(u1, v1));
			uv->push_back(Vector2f(u1, v0));

			layers->push_back(w);
			layers->push_back(w);
			layers->push_back(w);
			layers->push_back(w);

			// Move pointer
			charX += ax * ratioW;
			charY += ay * ratioH;
		}
		break;
	case BOT_RIGHT:
	case TOP_RIGHT:
		std::reverse(str.begin(), str.end());
		for (auto const& c : str)
		{
			// Texture parameters
			GLuint w;
			float u0, u1, v0, v1;
			// Glyph parameters
			float ax, ay, sx, sy, mx, my;

			bool hasVolume = font->getChar(&w,
			                               &u0, &u1,
			                               &v0, &v1,
			                               &ax, &ay,
			                               &sx, &sy,
			                               &mx, &my,
			                               c);
			if (!hasVolume) continue;

			flag = true;
			// Insert Geometry into graphics

			float x2 = charX + mx * ratioW;
			float y2 = charY + my * ratioH;
			float charW = sx * ratioW;
			float charH = sy * ratioH;

			vertices->push_back(Vector2f(x2,         y2));
			vertices->push_back(Vector2f(x2,         y2 - charH));
			vertices->push_back(Vector2f(x2 + charW, y2 - charH));
			vertices->push_back(Vector2f(x2 + charW, y2));
			uv->push_back(Vector2f(u0, v0));
			uv->push_back(Vector2f(u0, v1));
			uv->push_back(Vector2f(u1, v1));
			uv->push_back(Vector2f(u1, v0));

			layers->push_back(w);
			layers->push_back(w);
			layers->push_back(w);
			layers->push_back(w);

			// Move pointer
			charX -= ax * ratioW;
			charY -= ay * ratioH;
		}
		break;
	default:
		assert(false && "class Text: Unhandled align case");
	}

	return flag;
}

} // namespace fab
