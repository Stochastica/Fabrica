#include "TextureManager.hpp"

namespace fab
{

TextureManager::TextureManager(int blockSize):
	blockSize(blockSize),
	textureBlock(blockSize, blockSize, 256, 256, 16)
{
}


} // namespace fab
