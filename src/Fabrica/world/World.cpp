#include "World.hpp"

#include "../core/Fabrica.hpp"

namespace fab
{

World::World()
{
	for (int i = 0; i < Chunk::size; ++i)
		for (int j = 0; j < Chunk::size; ++j)
			for (int k = 0; k < Chunk::size; ++k)
				fillChunk.setBlock(Vector3i(i,j,k), &Fabrica::blockGrass);
}

} // namespace fab
