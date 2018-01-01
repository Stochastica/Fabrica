#include "Chunk.hpp"

namespace fab
{

constexpr int const Chunk::size;

Chunk::Chunk()
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			for (int k = 0; k < size; ++k)
				blocks[i][j][k] = &BlockNull::instance;
}

} // namespace fab
