#include "RenderingRegistry.hpp"

namespace fab
{

std::map<Block*, RenderBlock*> RenderingRegistry::renderBlocks;

TextureManager* RenderingRegistry::initAll()
{
	TextureManager* tm = new TextureManager();

	tm->getTextureBlock().loadTexture();
	int index = 0;
	auto blockTextureLoader = [&index, tex = &tm->getTextureBlock()](void* pixels)
	{
		tex->loadChart(index, pixels);
		return index++;
	};
	for (auto& r: renderBlocks)
		r.second->loadTextures(tm->getBlockSize(), blockTextureLoader);

	return tm;
}

} // namespace fab
