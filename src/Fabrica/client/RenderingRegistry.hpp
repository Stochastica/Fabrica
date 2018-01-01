#ifndef FABRICA_CLIENT_RENDERINGREGISTRY_HPP_
#define FABRICA_CLIENT_RENDERINGREGISTRY_HPP_

#include <cassert>
#include <map>
#include <mutex>

#include "../block/Block.hpp"
#include "renderer/block/RenderBlock.hpp"
#include "renderer/TextureManager.hpp"

namespace fab
{

/**
 * @brief Associates logical parts with rendering parts.
 */
class RenderingRegistry final
{
public:
	/**
	 * @brief Binds {@code renderBlock} to {@code block}
	 *
	 * Every time a {@code block} needs to be draw, the corresponding methods in
	 * {@code renderBlock} are called.
	 */
	static void registerRenderBlock(Block* block, RenderBlock* renderBlock);

	static RenderBlock* getRenderer(Block* block);

	/**
	 * @brief Load each renderer's texture into the TextureManager.
	 */
	static TextureManager* initAll();

private:
	RenderingRegistry() = delete;

	static std::map<Block*, RenderBlock*> renderBlocks;
};

// Implementations

inline void
RenderingRegistry::registerRenderBlock(Block* block, RenderBlock* renderBlock)
{
	renderBlocks[block] = renderBlock;
}

inline RenderBlock*
RenderingRegistry::getRenderer(Block* block)
{
	assert(renderBlocks.find(block) != renderBlocks.end() &&
	       "Block has no corresponding RenderBlock.");
	return renderBlocks[block];
}

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERINGREGISTRY_HPP_
