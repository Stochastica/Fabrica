#ifndef FABRICA_BLOCK_BLOCK_HPP_
#define FABRICA_BLOCK_BLOCK_HPP_

#include "../util/facing.hpp"

namespace fab
{

/**
 * @brief Base class of all blocks.
 */
class Block
{
public:
	virtual ~Block() = default;

	virtual bool isOpaque() const noexcept { return true; }
	virtual bool isSideSolid(Facing3) const noexcept { return isOpaque(); }
};

class BlockNull final: public Block
{
public:
	BlockNull() = default;
	virtual bool isOpaque() const noexcept override { return false; }

	static BlockNull instance;
};

} // namespace fab

#endif // !FABRICA_BLOCK_BLOCK_HPP_
