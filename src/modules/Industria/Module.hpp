#ifndef INDUSTRIA__MODULE_HPP_
#define INDUSTRIA__MODULE_HPP_

#include <boost/config.hpp>

#include <Fabrica/api/Module.hpp>

namespace industria
{

class Industria final: public fab::Module
{
public:
	std::string name() const noexcept override;
	std::string version() const noexcept override;

	void onPreInit(fab::StateEventPreInit&) override;
	void onClientInit(fab::StateEventClientInit&) override;
};

extern "C" BOOST_SYMBOL_EXPORT Industria module;
Industria module;

} // namespace industria

#endif // !INDUSTRIA__MODULE_HPP_
