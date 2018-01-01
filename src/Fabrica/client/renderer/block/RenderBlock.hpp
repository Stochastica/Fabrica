#ifndef FABRICA_CLIENT_RENDERER_BLOCK_RENDERBLOCK_HPP_
#define FABRICA_CLIENT_RENDERER_BLOCK_RENDERBLOCK_HPP_

#include <map>

#include "../../../world/World.hpp"
#include "../../../util/ResourceLocation.hpp"
#include "../Render.hpp"
#include "../TextureAtlas.hpp"

namespace fab
{

/**
 * @brief Each instance handles the drawing of a type of block.
 *
 * Not intended for drawing complex models.
 */
class RenderBlock
{
public:
	virtual ~RenderBlock() = default;

	/**
	 * @brief Called upon initialisation. Loads the texture using the given
	 *  registry function.
	 * @param[in] size Width/Height of each texture. At the moment = 16.
	 * @param registry A function, accepting a size x size array of RGBA
	 *  (1 byte/channel, 4 channels)
	 */
	virtual void loadTextures(int size, std::function<int (void*)> registry) = 0;

	/**
	 * @brief Loads the geometry (for Items and other effects)
	 *
	 * The geometry must be loaded between the vertices (0,0,0) and (1,1,1)
	 */
	virtual void loadGeometry(TextureAtlas const& atlas,
	                          GeometryLoader&) const = 0;
	/**
	 * @brief Loads the geometry (for World).
	 */
	virtual void loadGeometry(WorldIn* const world,
	                          Vector3i const& coordinate,
	                          TextureAtlas const& atlas,
	                          GeometryLoader&) const = 0;
};

class RenderBlockFull final: public RenderBlock
{
public:
	RenderBlockFull(); ///< Default constructor.
	RenderBlockFull(std::map<ResourceLocation, Facing3> const&);

	virtual void loadTextures(int size, std::function<int (void*)> registry) override final;

	virtual void loadGeometry(TextureAtlas const& atlas,
	                          GeometryLoader&) const override final;
	virtual void loadGeometry(WorldIn* const world,
	                          Vector3i const& coordinate,
	                          TextureAtlas const& atlas,
	                          GeometryLoader&) const override final;

private:
	std::map<ResourceLocation, Facing3> faces;
	int chartIds[6]; // 6 faces
};

/**
 * @brief Full block, all sides are the same.
 */
class RenderBlockUniform final: public RenderBlock
{
public:
	RenderBlockUniform(); ///< Default constructor.
	RenderBlockUniform(ResourceLocation const&);

	virtual void loadTextures(int size, std::function<int (void*)> registry) override final;

	virtual void loadGeometry(TextureAtlas const& atlas,
	                          GeometryLoader&) const override final;
	virtual void loadGeometry(WorldIn* const world,
	                          Vector3i const& coordinate,
	                          TextureAtlas const& atlas,
	                          GeometryLoader&) const override final;

private:
	ResourceLocation rl;
	unsigned int chartId;
};

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_BLOCK_RENDERBLOCK_HPP_
