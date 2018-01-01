#ifndef FABRICA_CLIENT_RENDERER_TEXTUREMANAGER_HPP_
#define FABRICA_CLIENT_RENDERER_TEXTUREMANAGER_HPP_

#include "TextureAtlas.hpp"

namespace fab
{

/**
 * @brief Stores persistent textures
 */
class TextureManager final
{
public:
	TextureManager(int blockSize = 16);

	int getBlockSize() const noexcept { return blockSize; }
	TextureAtlas& getTextureBlock() noexcept;
private:
	TextureAtlas textureBlock;

	int blockSize;
};

inline TextureAtlas&
TextureManager::getTextureBlock() noexcept
{
	return textureBlock;
}

} // namespace fab

#endif // !FABRICA_CLIENT_RENDERER_TEXTUREMANAGER_HPP_
