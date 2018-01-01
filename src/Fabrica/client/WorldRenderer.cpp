#include "WorldRenderer.hpp"

#include <iostream>

#include "RenderingRegistry.hpp"
#include "renderer/utils.hpp"

#include "../core/Fabrica.hpp"

namespace fab
{

InteractionFlags::InteractionFlags():
	showDebugScreen(true),
	trapCursor(false),
	drawWireframe(false)
{
}

GLuint WorldRenderer::program;
GLuint WorldRenderer::programPTransform;

void WorldRenderer::init()
{
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
	bool flag = registerGLProgram(&program, sourceVert, sourceFrag);
	assert(flag && "class WorldRenderer: GL Program register failed");
	(void) flag;
	programPTransform = glGetUniformLocation(program, "transform");
}
WorldRenderer::WorldRenderer(Window* const window,
                             WorldIn* const world,
                             std::mutex* const mutexWorld,
                             InteractionFlags* const interactions,
                             TextureManager* const textureManager):
	WindowListener(window),
	window(window),
	world(world),
	mutexWorld(mutexWorld),
	interactions(interactions),
	textureManager(textureManager),
	radiusXZ(6), radiusY(6),
	nChunks((2 * radiusXZ - 1) * (2 * radiusY - 1) * (2 * radiusXZ - 1)),
	chunkLoadOrder(genChunkLoadOrder(radiusXZ, radiusY))
{
	// Loads the chunkGeometries
	chunkGeometries = new ChunkGeometry** [radiusXZ * 2 - 1];
	for (int x = 0; x < 2 * radiusXZ - 1; ++x)
	{
		chunkGeometries[x] = new ChunkGeometry*[radiusY * 2 - 1];
		for (int y = 0; y < 2 * radiusY - 1; ++y)
		{
			chunkGeometries[x][y] = new ChunkGeometry[radiusXZ * 2 - 1];
			for (int z = 0; z < 2 * radiusXZ - 1; ++z)
			{
				glGenBuffers(1, &chunkGeometries[x][y][z].bufferVert);
				glGenBuffers(1, &chunkGeometries[x][y][z].bufferInd);
			}
		}
	}
}
WorldRenderer::~WorldRenderer()
{
	for (int x = 0; x < 2 * radiusXZ - 1; ++x)
	{
		for (int y = 0; y < 2 * radiusY - 1; ++y)
		{
			for (int z = 0; z < 2 * radiusXZ - 1; ++z)
			{
				glDeleteBuffers(1, &chunkGeometries[x][y][z].bufferVert);
				glDeleteBuffers(1, &chunkGeometries[x][y][z].bufferInd);
			}
			delete[] chunkGeometries[x][y];
		}
		delete[] chunkGeometries[x];
	}
	delete[] chunkGeometries;
	delete[] chunkLoadOrder;
}

void WorldRenderer::onKey(Key key, KeyAction ka, KeyMod)
{
	if (ka == KeyAction::Press)
	{
		if (key == Key::F3) // Toggle debug screen
		{
			std::cout << "Toggle Debug Screen\n";
			interactions->showDebugScreen =
			  !interactions->showDebugScreen;
		}
		else if (key == Key::Escape) // Trap cursor
		{
			interactions->trapCursor =
			  !interactions->trapCursor;
			window->cursorLock(interactions->trapCursor);
			if (interactions->trapCursor)
				window->cursorConfine();
		}
		else if (key == Key::F2) // Enable wireframe
		{
			interactions->drawWireframe =
			  !interactions->drawWireframe;
			glPolygonMode(GL_FRONT, interactions->drawWireframe ? GL_LINE : GL_FILL);
		}
	}
}
void WorldRenderer::onMouse(MouseButton, bool press, KeyMod)
{
	// Does nothing...
}
void WorldRenderer::loadChunk(Vector3i const& offset,
                              Vector3i const& pRelative)
{
	assert(-radiusXZ < pRelative.x() && pRelative.x() < radiusXZ);
	assert(-radiusY < pRelative.y() && pRelative.y() < radiusY);
	assert(-radiusXZ < pRelative.z() && pRelative.z() < radiusXZ);

	ChunkGeometry& cg = chunkGeometries[pRelative.x()][pRelative.y()][pRelative.z()];

	Vector3i const pAbsolute = offset + pRelative;
	Vector3i const pBase = Chunk::size * pAbsolute;
	ChunkIn* chunk = world->getChunkO(pAbsolute);
	if (!chunk)
	{
		cg.draw = false;
		return; // No Geometry update needed.
	}
	cg.draw = true;
	cg.vertices.clear();
	cg.indices.clear();

	// Iterate through blocks
	GeometryLoaderChunk gl(&cg);
	for (int x = 0; x < Chunk::size; ++x)
		for (int y = 0; y < Chunk::size; ++y)
			for (int z = 0; z < Chunk::size; ++z)
			{
				// Block* b = chunk->getBlock(x, y, z);
				RenderBlock* rb = RenderingRegistry::getRenderer(&Fabrica::blockGrass);
				rb->loadGeometry(world, pBase + Vector3i(x,y,z),
				                 textureManager->getTextureBlock(), gl);
				gl.nextOffset();
			}
}
void WorldRenderer::draw(Camera const& camera)
{
	// Read data from the world
	mutexWorld->lock();

	interactions->moveFront = window->isKeyPressed(Key::W);
	interactions->moveBack = window->isKeyPressed(Key::S);
	interactions->moveLeft = window->isKeyPressed(Key::A);
	interactions->moveRight = window->isKeyPressed(Key::D);
	interactions->moveUp = window->isKeyPressed(Key::Space);
	interactions->moveDown = window->isKeyPressed(Key::ShiftLeft);

	loadChunk(Vector3i(0,0,0), Vector3i(0,0,0));
	mutexWorld->unlock();

	// Drawing

	auto matrix = camera.matrix(window->aspectRatio());

	// Load all geometries
	glActiveTexture(GL_TEXTURE0);
	textureManager->getTextureBlock().loadTexture();
	glUseProgram(program);
	glUniformMatrix4fv(programPTransform, 1, GL_FALSE, &matrix[0][0]);

	ChunkGeometry& cg = chunkGeometries[0][0][0];
	{
		glEnableVertexAttribArray(0); // Position array
		glEnableVertexAttribArray(1); // UV array
		glEnableVertexAttribArray(2); // W array

		glBindBuffer(GL_ARRAY_BUFFER, cg.bufferVert);
		glBufferData(GL_ARRAY_BUFFER,
		             cg.vertices.size() * sizeof(RVertex),
		             cg.vertices.data(),
		             GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		                      sizeof(RVertex), (void*) offsetof(RVertex, p));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		                      sizeof(RVertex), (void*) offsetof(RVertex, uv));
		glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE,
		                      sizeof(RVertex), (void*) offsetof(RVertex, w));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cg.bufferInd);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		             cg.indices.size() * sizeof(unsigned int),
		             cg.indices.data(),
		             GL_DYNAMIC_DRAW);

		glDrawElements(GL_TRIANGLES, cg.indices.size(), GL_UNSIGNED_INT, nullptr);
	}


}

} // namespace fab

