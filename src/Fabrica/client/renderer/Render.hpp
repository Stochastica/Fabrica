#ifndef FABRICA_CLIENT_RENDERER_RENDER_HPP_
#define FABRICA_CLIENT_RENDERER_RENDER_HPP_

#include <vector>

#include "../../util/vector.hpp"
#include "../../util/facing.hpp"

namespace fab
{

/**
 * @brief "Rendering Vertex"
 */
struct RVertex
{
	Vector3f p; ///< Position
	Vector2f uv; ///< UV
	int w; ///< w coordinate
};

/**
 * Interface for geometry loading in Renderer objects.
 */
class GeometryLoader
{
public:
	virtual void loadVertex(RVertex const&) = 0;
	virtual void loadIndex(unsigned int) = 0;

	// The following functions shift the index offset by the necessary amount.
	void loadTri(int& offset, RVertex vertices[3]);
	void loadQuad(int& offset, RVertex vertices[4]);
	void loadBlockFace(int& offset, Vector3i const& coordinate, Facing3 face,
	                   float u0, float u1,
	                   float v0, float v1,
	                   unsigned int w);
};


// Implementations

inline void
GeometryLoader::loadTri(int& offset, RVertex vertices[3])
{
	loadVertex(vertices[0]);
	loadVertex(vertices[1]);
	loadVertex(vertices[2]);
	loadIndex(offset);
	loadIndex(offset + 1);
	loadIndex(offset + 2);
	offset += 3;
}
inline void
GeometryLoader::loadQuad(int& offset, RVertex vertices[4])
{
	loadVertex(vertices[0]);
	loadVertex(vertices[1]);
	loadVertex(vertices[2]);
	loadVertex(vertices[3]);
	loadIndex(offset);
	loadIndex(offset + 1);
	loadIndex(offset + 2);
	loadIndex(offset + 3);
	loadIndex(offset);
	loadIndex(offset + 2);
	offset += 4;
}
inline void
GeometryLoader::loadBlockFace(int& offset, Vector3i const& coordinate,
                              Facing3 face,
                              float u0, float u1,
                              float v0, float v1,
                              unsigned int w)
{
	int const x = coordinate.x();
	int const y = coordinate.y();
	int const z = coordinate.z();
	RVertex quad[4];
	switch (face)
	{
	case Facing3::Down:
		quad[0].p = Vector3f(x + 1, y, z);
		quad[1].p = Vector3f(x + 1, y, z + 1);
		quad[2].p = Vector3f(x, y, z + 1);
		quad[3].p = Vector3f(x, y, z);
		break;
	case Facing3::Up:
		quad[0].p = Vector3f(x, y + 1, z);
		quad[1].p = Vector3f(x, y + 1, z + 1);
		quad[2].p = Vector3f(x + 1, y + 1, z + 1);
		quad[3].p = Vector3f(x + 1, y + 1, z);
		break;
	case Facing3::North:
		quad[0].p = Vector3f(x + 1, y + 1, z);
		quad[1].p = Vector3f(x + 1, y, z);
		quad[2].p = Vector3f(x, y, z);
		quad[3].p = Vector3f(x, y + 1, z);
		break;
	case Facing3::South:
		quad[0].p = Vector3f(x, y + 1, z + 1);
		quad[1].p = Vector3f(x, y, z + 1);
		quad[2].p = Vector3f(x + 1, y, z + 1);
		quad[3].p = Vector3f(x + 1, y + 1, z + 1);
		break;
	case Facing3::West:
		quad[0].p = Vector3f(x, y + 1, z);
		quad[1].p = Vector3f(x, y, z);
		quad[2].p = Vector3f(x, y, z + 1);
		quad[3].p = Vector3f(x, y + 1, z + 1);
		break;
	case Facing3::East:
		quad[0].p = Vector3f(x + 1, y + 1, z + 1);
		quad[1].p = Vector3f(x + 1, y, z + 1);
		quad[2].p = Vector3f(x + 1, y, z);
		quad[3].p = Vector3f(x + 1, y + 1, z);
		break;
	}
	quad[0].uv = Vector2f(u0, v0);
	quad[1].uv = Vector2f(u0, v1);
	quad[2].uv = Vector2f(u1, v1);
	quad[3].uv = Vector2f(u1, v0);
	quad[0].w = quad[1].w = quad[2].w = quad[3].w = w;
	loadQuad(offset, quad);
}

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_RENDER_HPP_
