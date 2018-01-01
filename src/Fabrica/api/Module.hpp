#ifndef FABRICA_API_MODULE_HPP_
#define FABRICA_API_MODULE_HPP_

#include <cstdint>
#include <string>

#include <boost/core/noncopyable.hpp>

#include "../common/LogManager.hpp"
#include "stateevents/StateEventPreinit.hpp"
#include "stateevents/StateEventInit.hpp"
#include "stateevents/StateEventClientInit.hpp"

namespace fab
{

/**
 * @brief Base class of all modules, plugins that attach to Fabrica.
 * @warning Do not inherit directly from this class.
 *
 * Every Fabrica module must implement this interface to connect with the
 * dynamic module loading mechanism of Fabrica. A singleton of class {@code
 * Module} with name "module" should be provided within the module's namespace,
 * e.g.
 *
 * @code{.cpp}
 *
 * #include <boost/config.hpp> // Allows from BOOST_SYMBOL_EXPORT
 * #include <Fabrica/api/ModuleLogic.hpp>
 *
 * class Module: public fab::ModuleLogic
 * {
 * };
 *
 * extern "C" BOOST_SYMBOL_EXPORT Module module;
 * Module module
 *
 * @endcode
 *
 * where {@code ModuleLogic} can be replaced by one of the subtypes of Module.
 *
 * The {@code Logger logger} field in each {@code class Module} instance is
 * initialised to the desgnated logger for each Module. Do not attempt to
 * modify this field.
 *
 */
class Module: boost::noncopyable
{
public:
	virtual ~Module() = default;
	/**
	 * @brief Procedural name of the module.
	 * @warning Changing this breaks existing game saves.
	 *
	 * Must be unique enough to distinguish amongst other modules. Can only
	 * include [A-Za-z0-9], underscores, and |.
	 */
	virtual std::string name() const noexcept = 0;
	/**
	 * @brief Version of the module. Can only contain numbers, letters, and '.'
	 */
	virtual std::string version() const noexcept = 0;

	/**
	 * @brief Should the module be loaded on the client part?
	 */
	virtual bool hasClient() const noexcept { return true; }
	/**
	 * @brief Should the module be loaded on the server part?
	 */
	virtual bool hasServer() const noexcept { return true; }

	/**
	 * @brief Called upon pre-initialisation
	 *
	 * Only called in the server part.
	 */
	virtual void onPreInit(StateEventPreInit&) {}
	/**
	 * @brief Called upon initialisation
	 *
	 * Only called in the server part.
	 */
	virtual void onInit(StateEventInit&) {}

	/**
	 * @brief Called upon client initialisation
	 *
	 * Only called in the client part.
	 */
	virtual void onClientInit(StateEventClientInit&) {}

	Logger logger;
};

} // namespace fab

#endif // !FABRICA_API_MODULE_HPP_
