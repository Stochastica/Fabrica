#ifndef FABRICA_CORE_FABRICA_HPP_
#define FABRICA_CORE_FABRICA_HPP_

#include "../common/fabrica.hpp"
#include "../api/Module.hpp"
#include "../client/renderer/block/RenderBlock.hpp"

#include "block/BlockGrass.hpp"

namespace fab
{

/**
 * @brief Adds vanilla Fabrica mechanics.
 * @warning This is the ONLY module that is allowed to not provide a static
 *	instance and store all its members as static objects.
 */
class Fabrica final: public Module
{
public:
	std::string name() const noexcept override { return FABRICA_NAME; }
	std::string version() const noexcept override { return FABRICA_VERSION; }

	void onPreInit(StateEventPreInit&) override;
	void onInit(StateEventInit&) override;
#ifndef FABRICA_SERVER_STANDALONE
	void onClientInit(StateEventClientInit&) override;
#endif

	// Fields
	static BlockGrass blockGrass;

private:
#ifndef FABRICA_SERVER_STANDALONE
	RenderBlockFull rBlockGrass;
#endif
};

} // namespace fab

#endif // !FABRICA_CORE_FABRICA_HPP_
