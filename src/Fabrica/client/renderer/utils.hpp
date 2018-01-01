#ifndef FABRICA_CLIENT_RENDERER_UTILS_HPP_
#define FABRICA_CLIENT_RENDERER_UTILS_HPP_

#include <GL/glew.h>

#include "../../util/ResourceLocation.hpp"

namespace fab
{

/**
 * Debugging Tool: The GL Error check macro.
 */
#ifdef NDEBUG
#define GL_ERROR_CHECK
#else
#define GL_ERROR_CHECK \
	{\
		switch(glGetError()) \
		{ \
		case GL_NO_ERROR:\
			break;\
		case GL_INVALID_ENUM:\
			assert(false && "GL Invalid Enum");\
			break;\
		case GL_INVALID_VALUE:\
			assert(false && "GL Invalid Value");\
			break;\
		case GL_INVALID_OPERATION:\
			assert(false && "GL Invalid Operation");\
			break;\
		case GL_INVALID_FRAMEBUFFER_OPERATION:\
			assert(false && "GL Invalid Frame Buffer Operation");\
			break;\
		case GL_OUT_OF_MEMORY:\
			assert(false && "GL Out of memory");\
			break;\
		default:\
			assert(false && "GL Other errors");\
			break;\
		};\
	}
#endif

/**
 * @brief Register a vertex or fragment shader
 * @param[out] shader Non-null. Filled with shader Id when successful.
 * @param[in] source The GLSL source of the shader
 * @param[in] type Either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
 * @return True if successful
 */
bool registerGLShader(
  GLuint* const shader,
  char const source[],
  GLenum const type);
/**
 * @brief Register a GL shader program.
 * @param[out] program Non-null. Filled with program if successful.
 * @param[in] sourceVert Source of the vertex shader
 * @param[in] sourceFrag Source of the fragment shader
 * @return True if successful.
 */
bool registerGLProgram(GLuint* const program,
                       char const sourceVert[],
                       char const sourceFrag[]);

/**
 * @brief Generates indices for drawing quads.
 * @warning User is responsible for generating the buffers
 *
 * The buffer will have the data [0,1,2, 3,0,2, 4,5,6, 7,4,6, ...]
 * @param[out] buffer Buffer to be filled.
 * @param[in] nQuads Number of quads.
 * @return Number of indices
 */
GLuint generateQuadElementIndices(GLuint* buffer, std::size_t nQuads);

/**
 * @brief Loads a RGBA8 PNG texture to the array out.
 * @warning Does NOT resize the texture if it doesn't match the dimensions
 *	width x height. 
 * @param[out] out Array that will store the texture.
 *  This routine is not responsible for allocations.
 *
 *  If the texture is missing, the default width x height texture will be
 *  loaded instead.
 *
 * @param[out] out Pixels will be filled here. This routine is not responsible
 *	for allocations and the size of out must be able to hold all the pixels.
 * @param[in] rl Location of file
 * @param[in] width Default width
 * @param[in] height Default height
 * TODO: Allow resizing. (See boost GIL documentation)
 */
void loadTexturePNG(std::uint8_t* const out,
                    ResourceLocation const& rl,
                    int width, int height);

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_UTILS_HPP_
