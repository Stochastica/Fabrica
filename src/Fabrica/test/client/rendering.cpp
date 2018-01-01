#include "rendering.hpp"

#include <cstddef>

#include "../../client/Camera.hpp"
#include "../../client/DebugScreen.hpp"
#include "../../client/RenderingRegistry.hpp"
#include "../../client/Window.hpp"
#include "../../client/WorldRenderer.hpp"
#include "../../client/renderer/PerformanceMonitor.hpp"
#include "../../client/renderer/Text.hpp"
#include "../../client/renderer/TextureAtlas.hpp"
#include "../../client/renderer/utils.hpp"
#include "../../client/renderer/block/RenderBlock.hpp"
#include "../../common/fabrica.hpp"
#include "../../common/ModuleLoader.hpp"

namespace fab
{

bool test_cr1()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialisation failed\n";
		return false;
	}
	// Window creation routine
	// See client/Window.cpp
	int w = 1024;
	int h = 768;

	// Window instance constructor
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(w, h, "Fabrica", nullptr, nullptr);

	if (!window)
	{
		std::cerr << "GLFW Window Creation Routine Failed\n";
		return false;
	}

	glViewport(0, 0, w, h);

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialisation failed";
		return false;
	}

	// Query parameters:
	// Must be done after context creation
	{
		GLint maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		GLint maxArrayTextureLayers;
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayTextureLayers);
		GL_ERROR_CHECK;

		std::cout << "Parameters:\n"
		          << " GL Version: "
		          << glGetString(GL_VERSION) << '\n'
		          << " Max Texture Size (>1024): "
		          << maxTextureSize << '\n'
		          << " Max Array Texture Layers (>256): "
		          << maxArrayTextureLayers << '\n';
	}

	// Drawing

	bool flag;

	{
		// Core VertexArrayObject
		GLuint vertexArray;
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
	}

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	static const GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(vertices),
	             vertices,
	             GL_STATIC_DRAW);
	char const sourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec3 vertPosition;"
	  "void main()"
	  "{"
	  "gl_Position = vec4(vertPosition, 1);"
	  "}";
	char const sourceFrag[] =
	  "#version 330 core\n"
	  "out vec4 color;"
	  "void main()"
	  "{"
	  "color = vec4(.3,.3,.3,1);"
	  "}";

	GLuint program;
	flag = registerGLProgram(&program, sourceVert, sourceFrag);
	if (!flag)
	{
		std::cerr << "GL Program register failed\n";
		return false;
	}

	/**
	 * In practice, this should be window->isOpen()
	 */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT, GL_FILL);

		glUseProgram(program);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
		  0, // Shader parameter Id
		  3, // Each vertex position has 3 dimensions
		  GL_FLOAT, // Value type
		  GL_FALSE, // Normalised?
		  0, // Stride
		  nullptr // Array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		// class Window

		//window->swapBuffers();
		//window->pollEvents();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return true;
}
bool test_cr2()
{
	bool flag;
	Window* window = &Window::instance();

	{
		// Must bind vertex array
		GLuint vertexArray;
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);
	}

	// Trying to load a font that is not default...
	Font fr;
	ResourceLocation fontLocation("fabrica", "font/DejaVuSansMono.ttf");

	auto fontPath = ModuleLoader::instance().resolveLocation(fontLocation);
	if (!fr.setFontFile(fontPath.c_str()))
	{
		std::cerr << "class Font: Failed to load default Font\n";
		return false;
	}
	fr.setFontSize(20);


	DebugScreen ds(&fr);

	std::string testString =
	  "Arma Virumque Cano\n"
	  "Troiae qui primus ab oris";
	Text text1(&fr);
	Text text2(&fr);
	Text text3(&fr);
	Text text4(&fr);
	text1.setContents(0.f, 0.f, Text::TOP_LEFT,
	                  testString);
	text2.setContents(0.f, 0.f, Text::TOP_RIGHT,
	                  testString);
	text3.setContents(0.f, 0.f, Text::BOT_LEFT,
	                  testString);
	text4.setContents(0.f, 0.f, Text::BOT_RIGHT,
	                  testString);

	// Unicode not supported yet.
	/*
	std::cout << "Unicode Literal: " << ((int) L'म') << '\n';
	*/

	while (window->isOpen())
	{
		glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glPolygonMode(GL_FRONT, GL_FILL);
		GL_ERROR_CHECK;

		float color[] = {1.f, 1.f, 1.f, 1.f};
		text1.draw(color);

		color[0] = 1.f;
		color[1] = 0.f;
		color[2] = 0.f;
		text2.draw(color);
		color[0] = 0.f;
		color[1] = 1.f;
		color[2] = 0.f;
		text3.draw(color);
		color[0] = 0.f;
		color[1] = 0.f;
		color[2] = 1.f;
		text4.draw(color);

		ds.updateGeometry();
		ds.draw();

		window->swapBuffers();
		window->pollEvents();
	}
	return true;
}
bool test_cr3()
{
	std::uint8_t layer[16 * 16 * 4];
	ResourceLocation rl("fabrica", "grass_side.png");
	std::cout << "Resolving Location: "
	          << ModuleLoader::instance().resolveLocation(rl).string()
	          << std::endl;

	loadTexturePNG(layer, rl, 16, 16);

	// Printing:

	char const hex[] =
	{
		'0','1','2','3',
		'4','5','6','7',
		'8','9','A','B',
		'C','D','E','F'
	};
	char const brightness[] =
	{
		' ','.','.',':',
		':','-','-','=',
		'+','*','*','#',
		'#','%','%','@',
	};

	// Change to {@code charMap = hex;} for different output
	char const* charMap = brightness;

	std::string out =
	  'R' + std::string(16, ' ') +
	  'G' + std::string(16, ' ') +
	  'B' + std::string(16, ' ') +
	  'A' + '\n';
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			char c = charMap[layer[4 * (i * 16 + j)] / 16];
			out += c;
		}
		out += ' ';
		for (int j = 0; j < 16; ++j)
		{
			char c = charMap[layer[4 * (i * 16 + j) + 1] / 16];
			out += c;
		}
		out += ' ';
		for (int j = 0; j < 16; ++j)
		{
			char c = charMap[layer[4 * (i * 16 + j) + 2] / 16];
			out += c;
		}
		out += ' ';
		for (int j = 0; j < 16; ++j)
		{
			char c = charMap[layer[4 * (i * 16 + j) + 3] / 16];
			out += c;
		}
		out += '\n';
	}
	std::cout << out;

	return true;
}
bool test_cr4()
{
	// class Client
	Window* window = &Window::instance();

	TextureAtlas atlas(16, 16, 4, 4, 2);

	atlas.loadTexture();
	{
		// Load individual charts
		std::uint8_t layer[16 * 16 * 4];
		ResourceLocation rl0("fabrica", "grass_side.png");
		loadTexturePNG(layer, rl0, 16, 16);
		atlas.loadChart(0, layer);
		ResourceLocation rl1("fabrica", "grass_top.png");
		loadTexturePNG(layer, rl1, 16, 16);
		atlas.loadChart(1, layer);
	}

	char const sourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec3 vertPosition;"
	  "layout(location = 1) in vec2 vertexUV;"
	  "layout(location = 2) in uint tIdIn;"
	  "out vec2 uv;"
	  "flat out uint tId;"
	  "void main()"
	  "{"
	  "gl_Position = vec4(vertPosition, 1);"
	  "uv = vertexUV;"
	  "tId = tIdIn;"
	  "}";
	char const sourceFrag[] =
	  "#version 330 core\n"
	  "in vec2 uv;"
	  "flat in uint tId;"
	  "out vec4 color;"
	  "uniform sampler2DArray sampler;"
	  "void main()"
	  "{"
	  "color = texture(sampler, vec3(uv, tId));"
	  "}";
	GLuint program;
	bool flag = registerGLProgram(&program, sourceVert, sourceFrag);
	if (!flag)
	{
		std::cerr << "GL Program register failed\n";
		return false;
	}

	// First vertex array, for Graphics
	GLuint vertexArray1;
	glGenVertexArrays(1, &vertexArray1);
	glBindVertexArray(vertexArray1);

	GLuint w0, w1;
	float u00, u01, v00, v01;
	float u10, u11, v10, v11;
	atlas.chartUVW(&w0, &u00, &u01, &v00, &v01, 0);
	atlas.chartUVW(&w1, &u10, &u11, &v10, &v11, 1);

	// Regarding Orientations...
	// Since we are looking towards positive Z,
	// The North orientation is flipped.
	float const vertices[] =
	{
		// North face orientation
		1, 1, 0,
		1, 0, 0,
		0, 0, 0,
		0, 1, 0,
		// South face orientation
		-1, 0, 0,
		-1, -1, 0,
		0, -1, 0,
		0, 0, 0,
		// Another South face orientation
		-.5, .5, 0,
		-.5, 0, 0,
		0, 0, 0,
		0, .5, 0
	};
	float const uv[] =
	{
		u00, v00,
		u00, v01,
		u01, v01,
		u01, v00,

		u00, v00,
		u00, v01,
		u01, v01,
		u01, v00,

		u10, v10,
		u10, v11,
		u11, v11,
		u11, v10
	};
	GLuint const layers[] =
	{
		0,0,0,0,0,0,0,0,w1,w1,w1,w1
	};
	GLuint const indices[] =
	{
		0,1,2,
		3,0,2,
		4,5,6,
		7,4,6,
		8,9,10,
		11,8,10
	};
	GLuint const indLength = 18;

	GLuint bufferVert, bufferUV, bufferLayers, bufferIndices;
	{
		glGenBuffers(1, &bufferVert);
		glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
		glBufferData(GL_ARRAY_BUFFER,
		             sizeof(vertices),
		             vertices,
		             GL_DYNAMIC_DRAW);
		glGenBuffers(1, &bufferUV);
		glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
		glBufferData(GL_ARRAY_BUFFER,
		             sizeof(uv),
		             uv,
		             GL_DYNAMIC_DRAW);
		glGenBuffers(1, &bufferLayers);
		glBindBuffer(GL_ARRAY_BUFFER, bufferLayers);
		glBufferData(GL_ARRAY_BUFFER,
		             sizeof(layers),
		             layers,
		             GL_STATIC_DRAW);
		glGenBuffers(1, &bufferIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		             sizeof(indices),
		             indices,
		             GL_STATIC_DRAW);
	}

	// Load into VAO
	{
		glEnableVertexAttribArray(0); // Position array
		glEnableVertexAttribArray(1); // UV array
		glEnableVertexAttribArray(2); // Texture id array

		glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, bufferLayers);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);

		/*
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		*/
	}

	// Second Vertex array, for DebugScreen
	GLuint vertexArray2;
	glGenVertexArrays(1, &vertexArray2);

	Font const& fr = Font::defaultFont();
	DebugScreen ds(&fr);

	{
		glBindVertexArray(vertexArray2);
		ds.updateGeometry();
	}

	while (window->isOpen())
	{
		glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GL_ERROR_CHECK;

		glBindVertexArray(vertexArray1);
		atlas.loadTexture();
		glUseProgram(program);
		glActiveTexture(GL_TEXTURE0);
		GL_ERROR_CHECK;

		/*
		glEnableVertexAttribArray(0); // Position array
		glEnableVertexAttribArray(1); // UV array
		glEnableVertexAttribArray(2); // Texture id array

		glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, bufferLayers);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		*/
		glDrawElements(GL_TRIANGLES, indLength, GL_UNSIGNED_INT, nullptr);
		/*
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		*/

		glBindVertexArray(vertexArray2);
		ds.draw();

		GL_ERROR_CHECK;

		window->swapBuffers();
		window->pollEvents();
	}
	// Other rendering codes go here....
	return true;
}
bool test_cr5()
{
	// class Client
	Window* window = &Window::instance();

	TextureAtlas atlas(16, 16, 4, 4, 2);
	atlas.loadTexture();
	{
		// Load individual charts
		std::uint8_t layer[16 * 16 * 4];
		ResourceLocation rl1("fabrica", "grass_side.png");
		loadTexturePNG(layer, rl1, 16, 16);
		atlas.loadChart(1, layer);
		ResourceLocation rl2("fabrica", "grass_top.png");
		loadTexturePNG(layer, rl2, 16, 16);
		atlas.loadChart(2, layer);
		ResourceLocation rl0("fabrica", "dirt.png");
		loadTexturePNG(layer, rl0, 16, 16);
		atlas.loadChart(0, layer);
	}

	char const sourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec3 vertPosition;"
	  "layout(location = 1) in vec2 vertexUV;"
	  "layout(location = 2) in uint layer;"
	  "out vec2 uv;"
	  "flat out uint w;"
	  "uniform mat4 transform;"
	  "void main()"
	  "{"
	  "gl_Position = transform * vec4(vertPosition, 1);"
	  "uv = vertexUV;"
	  "w = layer;"
	  "}";
	char const sourceFrag[] =
	  "#version 330 core\n"
	  "in vec2 uv;"
	  "flat in uint w;"
	  "out vec4 color;"
	  "uniform sampler2DArray sampler;"
	  "void main()"
	  "{"
	  "color = texture(sampler, vec3(uv, w));"
	  "}";
	GLuint program;
	bool flag = registerGLProgram(&program, sourceVert, sourceFrag);
	if (!flag)
	{
		std::cerr << "GL Program register failed\n";
		return false;
	}
	GLuint programPTransform = glGetUniformLocation(program, "transform");


	GLuint worldVertexArray;
	glGenVertexArrays(1, &worldVertexArray);
	glBindVertexArray(worldVertexArray);

	// Load Geometry
	GLuint bufferVert, bufferUV, bufferLayers, bufferIndices;
	glGenBuffers(1, &bufferVert);
	glGenBuffers(1, &bufferUV);
	glGenBuffers(1, &bufferLayers);
	glGenBuffers(1, &bufferIndices);

	std::vector<Vector3f> vertices;
	std::vector<Vector2f> uv;
	std::vector<GLuint> layers;
	Vector3f basis(-.5f, -.5f, -.5f);

	/*
	 * Depend on the direction, push four vertices in the counterclockwise
	 * direction
	 *
	 * 0 <-- 3
	 * |     ^
	 * v     |
	 * 1 --> 2
	 *
	 * The top edge faces either north (for top and bottom) or up.
	 */

	// Down
	vertices.push_back(basis + Vector3f(1, 0, 0));
	vertices.push_back(basis + Vector3f(1, 0, 1));
	vertices.push_back(basis + Vector3f(0, 0, 1));
	vertices.push_back(basis + Vector3f(0, 0, 0));
	// Up
	vertices.push_back(basis + Vector3f(0, 1, 0));
	vertices.push_back(basis + Vector3f(0, 1, 1));
	vertices.push_back(basis + Vector3f(1, 1, 1));
	vertices.push_back(basis + Vector3f(1, 1, 0));
	// North
	vertices.push_back(basis + Vector3f(1, 1, 0));
	vertices.push_back(basis + Vector3f(1, 0, 0));
	vertices.push_back(basis + Vector3f(0, 0, 0));
	vertices.push_back(basis + Vector3f(0, 1, 0));
	// South
	vertices.push_back(basis + Vector3f(0, 1, 1));
	vertices.push_back(basis + Vector3f(0, 0, 1));
	vertices.push_back(basis + Vector3f(1, 0, 1));
	vertices.push_back(basis + Vector3f(1, 1, 1));
	// West
	vertices.push_back(basis + Vector3f(0, 1, 0));
	vertices.push_back(basis + Vector3f(0, 0, 0));
	vertices.push_back(basis + Vector3f(0, 0, 1));
	vertices.push_back(basis + Vector3f(0, 1, 1));
	// East
	vertices.push_back(basis + Vector3f(1, 1, 1));
	vertices.push_back(basis + Vector3f(1, 0, 1));
	vertices.push_back(basis + Vector3f(1, 0, 0));
	vertices.push_back(basis + Vector3f(1, 1, 0));

	// Load charts
	GLuint w;
	float u0, u1, v0, v1;
	atlas.chartUVW(&w, &u0, &u1, &v0, &v1, 0);
	uv.push_back(Vector2f(u0, v0));
	uv.push_back(Vector2f(u0, v1));
	uv.push_back(Vector2f(u1, v1));
	uv.push_back(Vector2f(u1, v0));
	layers.push_back(w);
	atlas.chartUVW(&w, &u0, &u1, &v0, &v1, 2);
	uv.push_back(Vector2f(u0, v0));
	uv.push_back(Vector2f(u0, v1));
	uv.push_back(Vector2f(u1, v1));
	uv.push_back(Vector2f(u1, v0));
	layers.push_back(w);
	for (int i = 0; i < 4; ++i)
	{
		atlas.chartUVW(&w, &u0, &u1, &v0, &v1, 1);
		uv.push_back(Vector2f(u0, v0));
		uv.push_back(Vector2f(u0, v1));
		uv.push_back(Vector2f(u1, v1));
		uv.push_back(Vector2f(u1, v0));
		layers.push_back(w);
	}

	GLuint indLength;

	{
		glEnableVertexAttribArray(0); // Position array
		glEnableVertexAttribArray(1); // UV array
		glEnableVertexAttribArray(2); // Texture id array

		glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
		glBufferData(GL_ARRAY_BUFFER,
		             vertices.size() * sizeof(Vector3f),
		             vertices.data(),
		             GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, bufferUV);
		glBufferData(GL_ARRAY_BUFFER,
		             uv.size() * sizeof(Vector2f),
		             uv.data(),
		             GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, bufferLayers);
		glBufferData(GL_ARRAY_BUFFER,
		             layers.size() * sizeof(GLuint),
		             layers.data(),
		             GL_DYNAMIC_DRAW);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 0, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		indLength = generateQuadElementIndices(&bufferIndices, vertices.size());
	}

	// Debug Screen
	Font const& font = Font::defaultFont();
	DebugScreen ds(&font);

	GLuint screenVertexArray;
	glGenVertexArrays(1, &screenVertexArray);
	glBindVertexArray(screenVertexArray);

	// Camera
	Camera camera;

	std::cout << "Model Initialisation Complete\n";

	// FPS Calculation
	PerformanceMonitor pm;
	while (window->isOpen())
	{
		pm.update();

		// Calculate camera parameters
		double cursorX, cursorY;
		window->getCursor(&cursorX, &cursorY);
		camera.setYawPitch((cursorX - window->getWidth() / 2) * 2 * M_PI /
		                   Window::instance().getWidth(),
		                   (cursorY - window->getHeight() / 2) * M_PI /
		                   Window::instance().getHeight());

		float const r = 5.f;
		float const z = r * std::cos(camera.getPitch()) * std::cos(camera.getYaw());
		float const x = r * std::cos(camera.getPitch()) * std::sin(-camera.getYaw());
		float const y = r * std::sin(camera.getPitch());
		camera.setPosition(-x, -y, -z);

		glm::mat4 mCamera = camera.matrix(window->aspectRatio());

		// Drawing routine

		glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GL_ERROR_CHECK;

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);

		// Draw world

		glBindVertexArray(worldVertexArray);

		glActiveTexture(GL_TEXTURE0);
		atlas.loadTexture();
		glUseProgram(program);
		glUniformMatrix4fv(programPTransform, 1, GL_FALSE, &mCamera[0][0]);
		glDrawElements(GL_TRIANGLES, indLength, GL_UNSIGNED_INT, nullptr);

		glDisable(GL_DEPTH_TEST);

		// Draw HUD

		glBindVertexArray(screenVertexArray);
		ds.setFPS(pm.getFPS());
		ds.setCameraData(camera);
		ds.updateGeometry();
		ds.draw();

		window->swapBuffers();
		window->pollEvents();
	}

	return true;
}
bool test_cr6()
{
	// class Client
	Window* window = &Window::instance();

	char const sourceVert[] =
	  "#version 330 core\n"
	  "layout(location = 0) in vec3 vertPosition;"
	  "layout(location = 1) in vec2 vertexUV;"
	  "layout(location = 2) in uint layer;"
	  "out vec2 uv;"
	  "flat out uint w;"
	  "uniform mat4 transform;"
	  "void main()"
	  "{"
	  "gl_Position = transform * vec4(vertPosition, 1);"
	  "uv = vertexUV;"
	  "w = layer;"
	  "}";
	char const sourceFrag[] =
	  "#version 330 core\n"
	  "in vec2 uv;"
	  "flat in uint w;"
	  "out vec4 color;"
	  "uniform sampler2DArray sampler;"
	  "void main()"
	  "{"
	  "color = texture(sampler, vec3(uv, w));"
	  "}";
	GLuint program;
	bool flag = registerGLProgram(&program, sourceVert, sourceFrag);
	if (!flag)
	{
		std::cerr << "GL Program register failed\n";
		return false;
	}
	GLuint programPTransform = glGetUniformLocation(program, "transform");

	std::map<ResourceLocation, Facing3> facemap;
	facemap[ResourceLocation("fabrica", "dirt.png")] = Facing3::Down;
	facemap[ResourceLocation("fabrica", "grass_side.png")] =
	  Facing3::North | Facing3::South | Facing3::West | Facing3::East;
	facemap[ResourceLocation("fabrica", "grass_top.png")] = Facing3::Up;
	RenderBlockFull rb(facemap);

	TextureAtlas atlas(16, 16, 4, 4, 2);
	int chartId = 0;

	rb.loadTextures(16, [&atlas, &chartId](void* array)
	{
		atlas.loadChart(chartId, array);
		return chartId++;
	});

	assert(chartId == 3 && "#Charts loaded is not 3");

	std::vector<RVertex> vertices;
	std::vector<unsigned int> indices;
	class GL_cr6 final: public GeometryLoader
	{
	public:
		GL_cr6(std::vector<RVertex>* vs, std::vector<unsigned int>* ind):
			vs(vs), ind(ind) {}

		void loadVertex(RVertex const& v) override
		{
			vs->push_back(v);
		}
		void loadIndex(unsigned int i) override
		{
			ind->push_back(i);
		}
	private:
		std::vector<RVertex>* vs;
		std::vector<unsigned int>* ind;
	};

	GL_cr6 gl(&vertices, &indices);

	rb.loadGeometry(atlas, gl);

	GLuint worldVertexArray;
	glGenVertexArrays(1, &worldVertexArray);
	glBindVertexArray(worldVertexArray);

	// Load Geometry into GL

	GLuint bufferVert, bufferIndices;
	glGenBuffers(1, &bufferVert);
	glGenBuffers(1, &bufferIndices);
	GLuint nInd;
	{
		glEnableVertexAttribArray(0); // Position array
		glEnableVertexAttribArray(1); // UV array
		glEnableVertexAttribArray(2); // W array

		glBindBuffer(GL_ARRAY_BUFFER, bufferVert);
		glBufferData(GL_ARRAY_BUFFER,
		             vertices.size() * sizeof(RVertex),
		             vertices.data(),
		             GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		                      sizeof(RVertex), (void*) offsetof(RVertex, p));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		                      sizeof(RVertex), (void*) offsetof(RVertex, uv));
		glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE,
		                      sizeof(RVertex), (void*) offsetof(RVertex, w));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		             indices.size() * sizeof(unsigned int),
		             indices.data(),
		             GL_DYNAMIC_DRAW);
	}

	assert(vertices.size() % 4 == 0);
	unsigned int nQuads = vertices.size() / 4;
	assert(indices.size() == nQuads * 6);

	std::cout << "Loaded " << nQuads << " quads.\n";

	// Debug Screen
	Font const& font = Font::defaultFont();
	DebugScreen ds(&font);

	GLuint screenVertexArray;
	glGenVertexArrays(1, &screenVertexArray);
	glBindVertexArray(screenVertexArray);
	std::cout << "Model Initialisation Complete\n";

	// Camera
	Camera camera;
	PerformanceMonitor pm;
	while (window->isOpen())
	{
		pm.update();

		// Calculate camera parameters
		double cursorX, cursorY;
		window->getCursor(&cursorX, &cursorY);
		camera.setYawPitch((cursorX - window->getWidth() / 2) * 2 * M_PI /
		                   Window::instance().getWidth(),
		                   (cursorY - window->getHeight() / 2) * M_PI /
		                   Window::instance().getHeight());

		float const r = 5.f;
		float const z = r * std::cos(camera.getPitch()) * std::cos(camera.getYaw());
		float const x = r * std::cos(camera.getPitch()) * std::sin(-camera.getYaw());
		float const y = r * std::sin(camera.getPitch());
		camera.setPosition(-x, -y, -z);

		glm::mat4 mCamera = camera.matrix(window->aspectRatio());

		// Drawing routine

		glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GL_ERROR_CHECK;

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);

		// Draw world

		glBindVertexArray(worldVertexArray);

		glActiveTexture(GL_TEXTURE0);
		atlas.loadTexture();
		glUseProgram(program);
		glUniformMatrix4fv(programPTransform, 1, GL_FALSE, &mCamera[0][0]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		glDisable(GL_DEPTH_TEST);

		// Draw HUD

		glBindVertexArray(screenVertexArray);
		ds.setFPS(pm.getFPS());
		ds.setCameraData(camera);
		ds.updateGeometry();
		ds.draw();

		window->swapBuffers();
		window->pollEvents();
	}
	return true;
}

bool test_cr7()
{
	Window* window = &Window::instance();
	World world;
	std::mutex mutex;
	InteractionFlags iflags;
	TextureManager* tm = RenderingRegistry::initAll();

	WorldRenderer wr(window, &world, &mutex, &iflags, tm);

	delete tm;
	return true;
}

} // namespace fab
