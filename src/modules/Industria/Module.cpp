#include "Module.hpp"

#include <iostream>

namespace industria
{

std::string Industria::name() const noexcept
{
	return "Industria";
}
std::string Industria::version() const noexcept
{
	return "1.2.3";
}

void Industria::onPreInit(fab::StateEventPreInit&)
{
	logger("Industria Pre-Init");
}
void Industria::onClientInit(fab::StateEventClientInit&)
{
}

} // namespace industria
