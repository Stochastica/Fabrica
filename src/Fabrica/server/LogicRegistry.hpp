#ifndef FABRICA_SERVER_LOGICREGISTRY_HPP_
#define FABRICA_SERVER_LOGICREGISTRY_HPP_

#include <map>
#include <mutex>
#include <string>

#include "../block/Block.hpp"
#include "../item/Item.hpp"

namespace fab
{

/**
 * A "uniform" is a singleton object that handles all in-game instances of its
 * kind e.g. {@code Item}, {@code Block}.
 *
 * @brief
 *	Stores all block and item uniforms, entity classes, world gens, etc.
 * @warning
 *	Modules should not use this class directly. Use StateEvents instead. This
 *	class does not manage the instantiation/destruction of uniforms.
 *	LogicRegistry is not responsible for deleting the pointers passed to it
 *	(since many prefer to have the Block/Item instances stored in the stack).
 *
 * To register a block, instantiate one instance and register with
 * {@code LogicRegistry::registerBlock}
 * To register an item, instantiate one instance and register with
 * {@code LogicRegistry::registerItem}
 *
 * {@code LogicRegistry} handles server side registering.
 */
class LogicRegistry final
{
public:
	/**
	 * @brief Thread-safe Block registration
	 * @warning Do not use this to register two blocks of the same name.
	 * @param[in] block
	 *	The block uniform to be registered.
	 * @param[in] name
	 *	The procedural name of this block.
	 */
	static void registerBlock(Block* block, std::string name);

private:
	LogicRegistry();
	~LogicRegistry();
	static LogicRegistry& instance();

	std::map<std::string, Block*> blocks;
	std::map<std::string, Item*> items;

	std::mutex blocksMutex;
	std::mutex itemsMutex;
};

// Implementations

inline void LogicRegistry::registerBlock(Block* block, std::string name)
{
	instance().blocksMutex.lock();
	instance().blocks[name] = block;
	instance().blocksMutex.unlock();
}

inline LogicRegistry& LogicRegistry::instance()
{
	static LogicRegistry inst;
	return inst;
}

} // namespace fab

#endif // !FABRICA_SERVER_LOGICREGISTRY_HPP_
