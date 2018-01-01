// Entry point of the application
#include <iostream>
#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/program_options.hpp>

#include "common/fabrica.hpp"
#include "common/ModuleLoader.hpp"

#ifdef FABRICA_SERVER_STANDALONE
	#include "server/standalone/ServerStandalone.hpp"
#else
	#include "client/Window.hpp"
	#include "client/Client.hpp"
	#include "client/ClientConfig.hpp"
#endif

#ifndef NDEBUG
	#include "test/test.hpp"
#endif

// Exit codes

#undef EXIT_NORMAL
#undef EXIT_INIT_ERROR
#undef EXIT_FAILURE

#define EXIT_NORMAL 0
#define EXIT_INIT_ERROR 1
#define EXIT_FAILURE 2

/**
 * @brief Detect if a path exists. If the path does not exist, try to create
 *  it.
 * @return True if the folder exists after execution.
 */
bool ensureFolderExists(boost::filesystem::path p)
{
	if (boost::filesystem::exists(p)) return true;

	boost::filesystem::create_directory(p);
	return boost::filesystem::exists(p);
}

int main(int argc, char* argv[])
{
	using namespace fab;

	boost::program_options::variables_map options;
	{
		namespace bpo = boost::program_options;

#ifndef NDEBUG
		std::string testDesc = "Execute test [arg]\n";
		for (auto const& i: testInfo())
			testDesc += i.first + ": " + i.second + '\n';
#endif

		/*
		 * Interpret command line arguemnts with boost::program_options
		 */
		bpo::options_description desc("Options");
		desc.add_options()
		("help,h", "Show help")
#ifndef NDEBUG
		("test,t", bpo::value<std::string>(), testDesc.c_str())
#endif
		;
		bpo::store(bpo::parse_command_line(argc, argv, desc), options);

		/*
		 * Depend on command line arguments, execute different routines.
		 */
		if (options.count("help"))
		{
			std::cout << desc << std::endl;
			return EXIT_NORMAL;
		}
	}

	// Find directories

	boost::filesystem::path
	pExecutable,
	pBase,
	pConfigs;

	pExecutable = boost::filesystem::complete(argv[0]);
	pBase = pExecutable.parent_path();
#ifdef FABRICA_SERVER_STANDALONE
	std::cout << "Loading Fabrica Server from " << pBase.string() << std::endl;
#else
	std::cout << "Loading Fabrica Client from " << pBase.string() << std::endl;
#endif
	{
		boost::filesystem::path pLogs = pBase / "logs";
		bool exists = ensureFolderExists(pLogs);
		if (!exists)
		{
			std::cerr << "Unable to create logs.\n";
			return EXIT_INIT_ERROR;
		}
		LogManager::instance().splitOutput((pLogs / "fabrica.log").string());
	}

	// Initialisation Logger
	Logger loggerInit = LogManager::create("Main");

	// Load configuration
	{
		pConfigs = pBase / "configs";
		bool exists = ensureFolderExists(pConfigs);
		if (!exists)
		{
			std::cerr << "Unable to create configurations.\n";
			return EXIT_INIT_ERROR;
		}
		// Modules folder
		boost::filesystem::path pModules = pBase / "modules";
		exists = boost::filesystem::exists(pModules);
		if (exists)
		{
			ModuleLoader::instance().findModules(pBase, pConfigs, pModules);
		}
		else
		{
			loggerInit.warn("Module path " + pModules.string() +
			                " does not exist."
			                " No modules will be loaded.");
		}
	}

	// Test routines
#ifndef NDEBUG
	if (options.count("test"))
	{
		std::string testId = options["test"].as<std::string>();
		std::cout << "-----Executing Test " << testId << "-----\n";
		bool result = test(testId);
		if (result)
			std::cout << "----- Test successful -----\n";
		else
			std::cout << "----- Test failed ------\n";

		return 0;
	}
#endif

	// Normal Execution

#ifdef FABRICA_SERVER_STANDALONE
	loggerInit(FABRICA_NAME " " FABRICA_VERSION " Server");
#else
	loggerInit(FABRICA_NAME " " FABRICA_VERSION);
#endif

	ModuleLoader::instance().load();

#ifdef FABRICA_SERVER_STANDALONE
	// Server Start
	
#else
	// Client start
	
	if (!glfwInit())
	{
		loggerInit.error("GLFW initialisation failed");
		return EXIT_INIT_ERROR;
	}
	loggerInit(std::string("GLFW Version: ") + glfwGetVersionString());

	ClientConfig clientConfig(pConfigs / "client.txt");
	clientConfig.read();

	{
		std::string error = Window::initAll(
		                      clientConfig.windowWidth,
		                      clientConfig.windowHeight
		                    );
		if (!error.empty())
		{
			loggerInit.error(error);
			return EXIT_INIT_ERROR;
		}
	}
	ModuleLoader::instance().preinit();
	ModuleLoader::instance().init();
	ModuleLoader::instance().clientInit();

	Client client(&clientConfig);
	loggerInit("Loading configuration file");
	client.reloadRenderers();

	World defaultWorld;
	client.loadWorld(&defaultWorld);
	client.execDraw();

	loggerInit("Writing configuration file");
	clientConfig.write();
#endif

	return EXIT_NORMAL;
}
