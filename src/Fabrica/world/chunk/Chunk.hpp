#ifndef FABRICA_WORLD_CHUNK_CHUNK_HPP_
#define FABRICA_WORLD_CHUNK_CHUNK_HPP_

#include "../../block/Block.hpp"
#include "../../util/vector.hpp"

namespace fab
{

/**
 * 16x16x16 area of block.
 */
class Chunk final
{
public:
	static constexpr int const size = 16;

	static Chunk& chunkNull() noexcept; ///< Chunk with all blocks being air.

	Chunk(); ///< Initialises the Chunk with BlockNull.

	void setBlock(Vector3i const& p, Block* const) noexcept;
	Block* getBlock(Vector3i const& p) const noexcept;
	Block* getBlock(int x, int y, int z) const noexcept;
private:
	Block* blocks[size][size][size];
};

typedef Chunk const ChunkIn; ///< Read only version of a chunk.

// Implementations

inline Chunk&
Chunk::chunkNull() noexcept
{
	static Chunk c;
	return c;
}
inline void
Chunk::setBlock(Vector3i const& p, Block* const b) noexcept
{
	blocks[p.x()][p.y()][p.z()] = b;
}
inline Block*
Chunk::getBlock(Vector3i const& p) const noexcept
{
	return getBlock(p.x(), p.y(), p.z());
}
inline Block*
Chunk::getBlock(int x, int y, int z) const noexcept
{
	assert(0 <= x && x < size);
	assert(0 <= y && y < size);
	assert(0 <= z && z < size);
	return blocks[x][y][z];
}

} // namespace fab

#endif // !FABRICA_WORLD_CHUNK_CHUNK_HPP_
