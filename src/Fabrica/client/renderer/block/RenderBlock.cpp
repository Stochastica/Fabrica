#include "RenderBlock.hpp"

#include "../utils.hpp"

namespace fab
{

RenderBlockFull::RenderBlockFull()
{
}
RenderBlockFull::RenderBlockFull(std::map<ResourceLocation, Facing3> const& faces):
	faces(faces)
{
	assert(!faces.empty() && "class RenderBlockFull: faces empty");
}

void RenderBlockFull::loadTextures(int size,
                                   std::function<int (void*)> registry)
{
	for (auto const& face: faces)
	{
		uint8_t tex[size * size * 4]; // 4 channels, RGBA
		loadTexturePNG(tex, face.first, size, size);
		int chartId = registry(tex);
		for (auto facing: FACING3_VALUES)
		{
			if (!isEmpty(facing & face.second))
				chartIds[toShortInt(facing)] = chartId;
		}
	}
}
void RenderBlockFull::loadGeometry(TextureAtlas const& atlas,
                                   GeometryLoader& loader) const
{
	float u0, u1, v0, v1;
	GLuint w;
	int offset = 0;

	for (Facing3 face: FACING3_VALUES)
	{
		atlas.chartUVW(&w, &u0, &u1, &v0, &v1, chartIds[toShortInt(face)]);
		loader.loadBlockFace(offset, Vector3i(0, 0, 0), face, u0, u1, v0, v1, w);
	}
}
void RenderBlockFull::loadGeometry(WorldIn* const world,
                                   Vector3i const& coordinate,
                                   TextureAtlas const& atlas,
                                   GeometryLoader& loader) const
{
	float u0, u1, v0, v1;
	GLuint w;
	int offset = 0;

	for (Facing3 face: FACING3_VALUES)
	{
		Block* adj = world->getBlockO(coordinate + toFace(face));
		if (adj && adj->isSideSolid(opposite(face)))
			continue; // Side solid. No need to push face.
		atlas.chartUVW(&w, &u0, &u1, &v0, &v1, chartIds[toShortInt(face)]);
		loader.loadBlockFace(offset, coordinate, face, u0, u1, v0, v1, w);
	}
}

RenderBlockUniform::RenderBlockUniform(): chartId(0) {}
RenderBlockUniform::RenderBlockUniform(ResourceLocation const& rl):
	rl(rl)
{
}
void RenderBlockUniform::loadTextures(int size,
                                      std::function<int (void*)> registry)
{
	uint8_t tex[size * size * 4]; // 4 channels, RGBA
	loadTexturePNG(tex, rl, size, size);
	chartId = registry(tex);
}
void RenderBlockUniform::loadGeometry(TextureAtlas const& atlas,
                                      GeometryLoader& loader) const
{
	float u0, u1, v0, v1;
	GLuint w;
	int offset = 0;
	atlas.chartUVW(&w, &u0, &u1, &v0, &v1, chartId);

	for (Facing3 face: FACING3_VALUES)
	{
		loader.loadBlockFace(offset, Vector3i(0, 0, 0), face, u0, u1, v0, v1, w);
	}
}
void RenderBlockUniform::loadGeometry(WorldIn* const world,
                                      Vector3i const& coordinate,
                                      TextureAtlas const& atlas,
                                      GeometryLoader& loader) const
{
	float u0, u1, v0, v1;
	GLuint w;
	int offset = 0;
	atlas.chartUVW(&w, &u0, &u1, &v0, &v1, chartId);

	for (Facing3 face: FACING3_VALUES)
	{
		Block* adj = world->getBlockO(coordinate + toFace(face));
		if (adj && adj->isSideSolid(opposite(face)))
			continue; // Side solid. No need to push face.
		loader.loadBlockFace(offset, coordinate, face, u0, u1, v0, v1, w);
	}
}

} // namespace fab
