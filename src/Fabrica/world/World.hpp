#ifndef FABRICA_WORLD_WORLD_HPP_
#define FABRICA_WORLD_WORLD_HPP_

#include "chunk/Chunk.hpp"
#include "../util/integers.hpp"

namespace fab
{

class World final
{
public:
	/**
	 * The only valid chunk coordinates [x,y,z] are:
	 * -chunkXMax <= x < chunkXMax
	 * -chunkZMax <= z < chunkZMax
	 * 0 <= y <= chunkYMax
	 */
	static constexpr int chunkXMax = 256;
	static constexpr int chunkZMax = 256;
	static constexpr int chunkYMax = 16;

	World();

	/**
	 * @brief Gets the chunk at the given position.
	 *
	 * Does not attempt to load the chunk. If the chunk is not loaded,
	 * returns nullptr.
	 */
	ChunkIn* getChunkO(Vector3i const& position) const noexcept;

	Block* getBlockO(Vector3i const& position) const noexcept;

private:
	Chunk fillChunk; ///< Filled with grass
};

typedef World const WorldIn; ///< Read only version of a world.

// Implementations

inline Block*
World::getBlockO(Vector3i const& p) const noexcept
{
	if (p.x() < -chunkXMax * Chunk::size ||
	    p.x() >= chunkXMax * Chunk::size ||
	    p.z() < -chunkZMax * Chunk::size ||
	    p.z() >= chunkZMax * Chunk::size ||
	    p.y() < 0 ||
	    p.y() >= chunkYMax * Chunk::size)
	{
		return nullptr;
	}
	ChunkIn* c = getChunkO(Vector3i(divide_floor(p.x(), Chunk::size),
	                                divide_floor(p.y(), Chunk::size),
	                                divide_floor(p.z(), Chunk::size)));
	if (c)
	{
		return c->getBlock(Vector3i(modulo_floor(p.x(), Chunk::size),
		                            modulo_floor(p.y(), Chunk::size),
		                            modulo_floor(p.z(), Chunk::size)));
	}
	else
	{
		return nullptr;
	}
}

inline ChunkIn*
World::getChunkO(Vector3i const& position) const noexcept
{
	if ((position.x() == 0) &&
			(position.y() == 0) &&
			(position.z() == 0))
		return &fillChunk;
	else
		return nullptr;
}

} // namespace fab

#endif // !FABRICA_WORLD_WORLD_HPP_
