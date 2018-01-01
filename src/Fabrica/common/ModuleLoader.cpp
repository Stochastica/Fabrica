#include "ModuleLoader.hpp"

#include <cassert>

#include <boost/dll/import.hpp>

#include "../core/Fabrica.hpp"

namespace fab
{

/**
 * @brief Returns true if a module's name is legal.
 *
 * See documentation of {@code Module} for legal module names.
 */
bool isModuleNameLegal(std::string name)
{
	if (name.empty()) return false;
	for (auto i = name.begin(); i != name.end(); ++i)
	{
		if (!std::isalpha(*i) &&
		    !std::isdigit(*i) &&
		    *i != '_' &&
		    *i != '|')
		{
			return false;
		}
	}
	return true;
}

boost::filesystem::path ModuleLoader::resolveDomain(std::string domain) const
{
	for (auto const& m: modules)
	{
		if (m.first->name() == domain)
			return m.second.basePath / "assets";
	}
	return defaultPath / "assets";
}
void ModuleLoader::findModules(
  boost::filesystem::path pCore,
  boost::filesystem::path pConfigs,
  boost::filesystem::path path)
{
	(void) pConfigs;
	// Add vanilla module
	{
		boost::shared_ptr<Fabrica> core(new Fabrica);

		ModuleInfo coreInfo;
		coreInfo.id = FABRICA_NAME;
		coreInfo.basePath = pCore;
		modules[core] = coreInfo;

		core->logger = LogManager::create(FABRICA_NAME);

		defaultPath = pCore;
	}

	// If modpath does not exist, exit and load no modules.
	if (!boost::filesystem::exists(path) ||
	    !boost::filesystem::is_directory(path))
	{
		logger.warn("Warning: Unable to find module path: " + path.string() +
		            ". No modules will be loaded");
		return;
	}

	// Iterate through folders within path/
	for (auto& subpath: boost::filesystem::directory_iterator(path))
	{
		// Skip non-paths
		if (!boost::filesystem::is_directory(subpath.path())) continue;

		// Iterate through files within subpath/
		for (auto& file: boost::filesystem::directory_iterator(subpath))
		{
			// Skip entries that are not loadable.
			if (!boost::filesystem::is_regular_file(file)) continue; // Not a file
			std::string extension = file.path().extension().string();
			if (extension != ".dll" && extension != ".so") continue; // Not a module

			modulePaths.push_back(file.path());
		}

	}

}
void ModuleLoader::load()
{
	for (auto const& path: modulePaths)
	{
		boost::shared_ptr<Module> module;
		try // Tries to load the module
		{
			// See Boost.DLL
			module = boost::dll::import<Module>(path, "module");
			logger("Name: " + module->name());
		}
		catch (std::exception const& e)
		{
			logger.warn("Unable to load module from " + path.string());
			logger(e.what());
			continue;
		}

		// Modules with illegal names cannot be loaded
		if (!isModuleNameLegal(module->name()))
		{
			logger.warn("Unable to load module: " + module->name() +
			            " contains illegal characters.");
			continue;
		}
		logger("Found module: " + module->name() + ' ' + module->version());
		module->logger = LogManager::create(module->name());
	}
}

void ModuleLoader::preinit()
{
	logger("Stage: Pre-Initialisation");

	StateEventPreInit event;
	for (auto& m: modules)
	{
		event.setName(m.first->name());
		m.first->onPreInit(event);
	}
}
void ModuleLoader::init()
{
	logger("Stage: Initialisation");
	StateEventInit event;
	for (auto& m: modules)
	{
		event.setName(m.first->name());
		m.first->onInit(event);
	}
}

#ifndef FABRICA_SERVER_STANDALONE
void ModuleLoader::clientInit()
{
	logger("Stage: Client Initialisation");
	StateEventClientInit event;
	for (auto& m: modules)
	{
		event.setName(m.first->name());
		m.first->onClientInit(event);
	}
}
#endif

} // namespace fab

