#include "utils.hpp"

#include <cassert>
#include <iostream>

/**
 * Defined to mamke PNG io work
 */
#define png_infopp_NULL (png_infopp) NULL
#define int_p_NULL (int*) NULL

#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

#include "../../common/ModuleLoader.hpp"

namespace fab
{

bool registerGLShader(
  GLuint* const shader,
  char const source[],
  GLenum type)
{
	// Compile the shader from source
	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &source, nullptr);
	glCompileShader(*shader);

	// If error occurs, return false

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		char* error = new char[infoLogLength + 1];
		glGetShaderInfoLog(*shader, infoLogLength, nullptr, error);
		std::cerr << error << std::endl;
		delete[] error;
		return false;
	}
	else
		return true;
}

bool registerGLProgram(GLuint* const program,
                       char const sourceVert[],
                       char const sourceFrag[])
{
	GLuint shaderVert, shaderFrag;

	// Compile the shaders
	bool flag = registerGLShader(&shaderVert, sourceVert,
	                             GL_VERTEX_SHADER);
	if (!flag)
	{
		std::cerr << "GL Vertex Shader register failed\n";
		return false;
	}
	flag = registerGLShader(&shaderFrag, sourceFrag,
	                        GL_FRAGMENT_SHADER);
	if (!flag)
	{
		std::cerr << "GL Fragment Shader register failed\n";
		return false;
	}

	// Register the program
	*program = glCreateProgram();
	glAttachShader(*program, shaderVert);
	glAttachShader(*program, shaderFrag);
	glLinkProgram(*program);

	// If error occurs, return false
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(*program, GL_LINK_STATUS, &result);
	glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		char* error = new char[infoLogLength + 1];
		glGetProgramInfoLog(*program, infoLogLength, nullptr, error);
		std::cerr << error << std::endl;
		delete[] error;
		return false;
	}

	// Clean up
	glDetachShader(*program, shaderVert);
	glDetachShader(*program, shaderFrag);
	glDeleteShader(shaderVert);
	glDeleteShader(shaderFrag);

	return true;
}
GLuint generateQuadElementIndices(GLuint* buffer, std::size_t nQuads)
{
	GLuint indices[nQuads * 6];
	for (std::size_t i = 0; i < nQuads; ++i)
	{
			GLuint basis = 4 * i;
			indices[i * 6]     = basis;
			indices[i * 6 + 1] = basis + 1;
			indices[i * 6 + 2] = basis + 2;
			indices[i * 6 + 3] = basis + 3;
			indices[i * 6 + 4] = basis;
			indices[i * 6 + 5] = basis + 2;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             sizeof(indices),
	             indices,
	             GL_DYNAMIC_DRAW);
	return nQuads * 6;
}

void loadTexture16PNG(std::uint8_t* const out,
                      ResourceLocation const& rl)
{
	// The absolute path to the texture
	std::string resolve = ModuleLoader::instance().resolveLocation(rl).string();

	try
	{
		boost::gil::rgba8_image_t image;
		boost::gil::png_read_image(resolve, image);
		auto view = boost::gil::view(image);
		typedef decltype(view)::value_type Pixel;

		static_assert(sizeof(Pixel) == 4, "Must be RGBA_8 format");

		boost::gil::copy_pixels(view,
		                        boost::gil::interleaved_view(
		                          view.width(),
		                          view.height(),
		                          (Pixel*) out,
		                          view.width() * sizeof(Pixel)
		                        )
		                       );
	}
	catch (std::ios_base::failure& e)
	{
		// Fails to read image. Use the default missing texture instead
		std::cout << e.what() << '\n';

		static int const dim = 16;
		// Order: RGBA
		/*
		static uint32_t const colourPurple = 0xFFFF00FF;
		static uint32_t const colourBlack = 0xFF000000;
		std::uint32_t* pixels = (std::uint32_t*) out;
		*/

		for (int i = 0; i < dim/2; ++i)
		{
			int line = i * dim;
			int line2 = (i + dim/2) * dim;
			for (int j = 0; j < dim/2; ++j)
			{
				// Top Left, Purple
				out[(line + j) * 4    ] = 0xFF;
				out[(line + j) * 4 + 1] = 0x00;
				out[(line + j) * 4 + 2] = 0xFF;
				out[(line + j) * 4 + 3] = 0xFF;

				// Bottom Left, Black
				out[(line2 + j) * 4    ] = 0x00;
				out[(line2 + j) * 4 + 1] = 0x00;
				out[(line2 + j) * 4 + 2] = 0x00;
				out[(line2 + j) * 4 + 3] = 0xFF;

				// Top Right, Black
				out[(line + j + dim/2) * 4    ] = 0x00;
				out[(line + j + dim/2) * 4 + 1] = 0x00;
				out[(line + j + dim/2) * 4 + 2] = 0x00;
				out[(line + j + dim/2) * 4 + 3] = 0xFF;

				// Bottom Right, Purple
				out[(line2 + j + dim/2) * 4    ] = 0xFF;
				out[(line2 + j + dim/2) * 4 + 1] = 0x00;
				out[(line2 + j + dim/2) * 4 + 2] = 0xFF;
				out[(line2 + j + dim/2) * 4 + 3] = 0xFF;

				/*
				pixels[line + j] = colourPurple; // Purple
				pixels[line2 + j] = colourBlack; // Black
				pixels[line + j + dim/2] = colourBlack; // Black
				pixels[line2 + j + dim/2] = colourPurple; // Purple
				*/
			}
		}
	}
}

void loadTexturePNG(std::uint8_t* const out,
                    ResourceLocation const& rl,
                    int width, int height)
{
	// The absolute path to the texture
	std::string resolve = ModuleLoader::instance().resolveLocation(rl).string();

	try
	{
		boost::gil::rgba8_image_t image;
		boost::gil::png_read_image(resolve, image);
		auto view = boost::gil::view(image);
		typedef decltype(view)::value_type Pixel;

		static_assert(sizeof(Pixel) == 4,
				"client/renderer/utils.cpp: "
				"Must be RGBA_8 format");

		boost::gil::copy_pixels(view,
		                        boost::gil::interleaved_view(
		                          view.width(),
		                          view.height(),
		                          (Pixel*) out,
		                          view.width() * sizeof(Pixel)
		                        )
		                       );
	}
	catch (std::ios_base::failure& e)
	{
		// Fails to read image. Use the default missing texture instead
		std::cout << e.what() << '\n';

		for (int i = 0; i < width; ++i)
		{
			bool flagI = ((i >> 3) & 1) == 1;
			for (int j = 0; j < height; ++j)
			{
				bool flagJ = ((j >> 3) & 1) == 1;

				int offset = (i * height + j) * 4;

				if (flagI == flagJ)
				{
					out[offset    ] = 0x00;
					out[offset + 1] = 0x00;
					out[offset + 2] = 0x00;
					out[offset + 3] = 0xFF;
				}
				else
				{
					out[offset    ] = 0xFF;
					out[offset + 1] = 0x00;
					out[offset + 2] = 0xFF;
					out[offset + 3] = 0xFF;
				}
			}
		}


	}
}


} // namespace fab
