#include "Fabrica.hpp"

#include <iostream>

#include "../client/RenderingRegistry.hpp"

namespace fab
{

BlockGrass Fabrica::blockGrass;

void Fabrica::onPreInit(StateEventPreInit&)
{
}
void Fabrica::onInit(StateEventInit& event)
{
	event.registerBlock(&blockGrass, "grass");
};
#ifndef FABRICA_SERVER_STANDALONE
void Fabrica::onClientInit(StateEventClientInit&)
{
	{
		std::map<ResourceLocation, Facing3> facemap;
		facemap[ResourceLocation("fabrica", "dirt.png")] = Facing3::Down;
		facemap[ResourceLocation("fabrica", "grass_side.png")] =
		  Facing3::North | Facing3::South | Facing3::West | Facing3::East;
		facemap[ResourceLocation("fabrica", "grass_top.png")] = Facing3::Up;
		rBlockGrass = RenderBlockFull(facemap);
		RenderingRegistry::registerRenderBlock(&blockGrass, &rBlockGrass);
	}
}
#endif

} // namespace fab
