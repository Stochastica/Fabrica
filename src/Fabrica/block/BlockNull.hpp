#ifndef FABRICA_BLOCK_BLOCKNULL_HPP_
#define FABRICA_BLOCK_BLOCKNULL_HPP_

#include "Block.hpp"

namespace fab
{

class BlockNull final: public Block
{
public:
	BlockNull() = default;
	virtual bool isOpaque() const noexcept override { return false; }

	static BlockNull instance;
};

BlockNull BlockNull::instance;

} // namespace fab

#endif // !FABRICA_BLOCK_BLOCKNULL_HPP_
