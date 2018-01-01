#ifndef FABRICA_API_STATEEVENTS_STATEEVENTINIT_HPP_
#define FABRICA_API_STATEEVENTS_STATEEVENTINIT_HPP_

#include "StateEvent.hpp"
#include "../../block/Block.hpp"
#include "../../item/Item.hpp"

namespace fab
{

/**
 * @brief State event associated with the initialisation stage.
 *
 * In this stage, blocks and items are registered via the corresponding methods
 * in this class.
 */
class StateEventInit final: public StateEvent
{
public:
	void registerBlock(Block*, std::string);
	//void registerItem(Item*, std::string);
};

} // namespace fab

#endif // !FABRICA_API_STATEEVENTS_STATEEVENTINIT_HPP_
