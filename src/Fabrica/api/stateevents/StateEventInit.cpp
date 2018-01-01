#include "StateEventInit.hpp"

#include "../../server/LogicRegistry.hpp"

namespace fab
{

void StateEventInit::registerBlock(Block* b, std::string name)
{
	LogicRegistry::registerBlock(b, modName + ':' + name);
}

} // namespace fab
