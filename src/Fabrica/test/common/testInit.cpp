#include "testInit.hpp"

#include <boost/dll/import.hpp>

#include "../testing.hpp"

#include "../../common/ModuleLoader.hpp"

namespace fab
{

bool test_i1()
{
	std::cout << "Potential Modules: \n";

	auto modulePaths = ModuleLoader::instance().getModulePaths();
	for (auto const& p: modulePaths)
	{
		std::cout << p.string() << '\n';
	}
	if (modulePaths.empty())
	{
		std::cout << "Test premise not met: No potential modules exist.\n";
		return false;
	}

	auto path = modulePaths[0];
	boost::shared_ptr<Module> module;
	try
	{
		module = boost::dll::import<Module>(path, "module");
		assert(module.get());
		std::cout << "Module Name: " << module->name() << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cout << e.what();
		return false;
	}
	
	// ModuleLoader::instance().load();
	return true;
}
bool test_i2()
{
	ModuleLoader::instance().load();
	ModuleLoader::instance().preinit();
	ModuleLoader::instance().init();
#ifndef FABRICA_SERVER_STANDALONE
	ModuleLoader::instance().clientInit();
#endif
	return true;
}

bool testInit(std::string id)
{
	TEST_FUNC(i1);
	TEST_FUNC(i2);

	TEST_FINAL;
}

} // namespace fab
