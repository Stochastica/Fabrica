#ifndef FABRICA_CLIENT_WORLDRENDERER_HPP_
#define FABRICA_CLIENT_WORLDRENDERER_HPP_

#include <mutex>
#include <vector>

#include "Camera.hpp"
#include "Window.hpp"
#include "renderer/Render.hpp"
#include "renderer/TextureManager.hpp"
#include "../util/facing.hpp"
#include "../world/World.hpp"

namespace fab
{

/**
 * @brief Sturct for storing the interaction data
 */
struct InteractionFlags
{
	InteractionFlags();

	// Persistent
	bool showDebugScreen;
	bool trapCursor;
	bool drawWireframe;

	// Transient
	bool moveLeft;
	bool moveRight;
	bool moveFront;
	bool moveBack;
	bool moveUp;
	bool moveDown;
};

struct ChunkGeometry
{
	ChunkGeometry(): draw(false) {}

	std::vector<RVertex> vertices;
	std::vector<unsigned int> indices;
	GLuint bufferVert;
	GLuint bufferInd;
	bool draw;
};

class GeometryLoaderChunk final: public GeometryLoader
{
public:
	GeometryLoaderChunk(ChunkGeometry* cg): cg(cg), offset(0) {}

	void loadVertex(RVertex const& v) override
	{
		cg->vertices.push_back(v);
	}
	void loadIndex(unsigned int i) override
	{
		cg->indices.push_back(i + offset);
	}
	void nextOffset()
	{
		offset = cg->vertices.size();
	}

private:
	ChunkGeometry* cg;
	unsigned int offset;
};

/**
 * One instance of this class in Client handles drawing the in-game world.
 */
class WorldRenderer final: public WindowListener
{
public:
	static void init();

	WorldRenderer(Window* const,
	              WorldIn* const,
	              std::mutex* const,
	              InteractionFlags* const,
	              TextureManager* const);
	~WorldRenderer();

	virtual void onKey(Key, KeyAction, KeyMod) override;
	virtual void onMouse(MouseButton, bool press, KeyMod) override;

	void loadChunk(Vector3i const& offset, Vector3i const& pRelative);
	/**
	 * @brief Draws the 3D component of the world.
	 */
	void draw(Camera const& mCamera);

private:
	Window* window;
	WorldIn* world;
	std::mutex* mutexWorld;
	InteractionFlags* interactions;
	TextureManager* textureManager;

	int radiusXZ, radiusY; ///< Radius for loading chunks
	int nChunks;
	Vector3i* chunkLoadOrder; ///< Order by which to load chunks
	ChunkGeometry*** chunkGeometries;

	static GLuint program;
	static GLuint programPTransform;
};

} // namespace fab

#endif // !FABRICA_CLIENT_WORLDRENDERER_HPP_
