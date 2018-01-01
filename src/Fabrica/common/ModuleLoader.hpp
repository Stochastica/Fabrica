#ifndef FABRICA_COMMON_MODULELOADER_HPP_
#define FABRICA_COMMON_MODULELOADER_HPP_

#include <string>
#include <map>
#include <vector>

#include <boost/core/noncopyable.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "../api/Module.hpp"
#include "../common/LogManager.hpp"
#include "../util/ResourceLocation.hpp"

namespace fab
{

/**
 * Stores commonly used information of modules.
 */
struct ModuleInfo
{
	std::string id;
	boost::filesystem::path basePath;
};
/**
 * @brief Singleton that Initialises modules.
 */
class ModuleLoader final: boost::noncopyable
{
public:
	/**
	 * @brief Converts the given domain string to a filesystem path.
	 * @warning Has overhead O(number of modules)
	 * @param[in] domain A domain name (a.k.a. the name of a module)
	 * @return The path to the folder containing the module.
	 *
	 * If the domain cannot be resolved, the path to the modules folder will be
	 * given.
	 */
	boost::filesystem::path resolveDomain(std::string domain) const;

	/**
	 * @brief Converts a {@code ResourceLocation} to a file in the filesystem.
	 */
	boost::filesystem::path resolveLocation(ResourceLocation const& rl) const;

	std::vector<boost::filesystem::path> getModulePaths() const noexcept;

	/**
	 * @brief Finds modules in the given directory.
	 * @warning Must be called after {@class LogManager} is initialised.
	 *
	 * After finding the modules, call {@code load} to load them.
	 */
	void findModules(boost::filesystem::path pCore,
	                 boost::filesystem::path pConfigs,
	                 boost::filesystem::path pModules);

	void load();

	// Initialisation stages
	
	void preinit();
	void init();

#ifndef FABRICA_SERVER_STANDALONE
	void clientInit();
#endif

	static ModuleLoader& instance();

private:
	ModuleLoader();

	Logger logger;
	boost::filesystem::path defaultPath;
	std::vector<boost::filesystem::path> modulePaths;

	std::map<boost::shared_ptr<Module>, ModuleInfo> modules;
};

// Implementations

inline boost::filesystem::path
ModuleLoader::resolveLocation(ResourceLocation const& rl) const
{
	boost::filesystem::path p(rl.getPath());
	return resolveDomain(rl.getDomain()) / p;
}
inline std::vector<boost::filesystem::path>
ModuleLoader::getModulePaths() const noexcept
{
	return modulePaths;
}
inline ModuleLoader& ModuleLoader::instance()
{
	static ModuleLoader inst;
	return inst;
}
inline ModuleLoader::ModuleLoader():
	logger(LogManager::create("Loader"))
{
}

} // namespace fab

#endif // !FABRICA_COMMON_MODULELOADER_HPP_
