#include "testClient.hpp"

// Testing Headers
#include "../testing.hpp"
#include "rendering.hpp"
#include "initialisation.hpp"

// Auxiliary Headers
#include "../../client/Window.hpp"
#include "../../common/ModuleLoader.hpp"

namespace fab
{

bool testClient(std::string id)
{
	TEST_FUNC(cr1);

	// main.cpp
	if (!glfwInit())
	{
		std::cerr << "GLFW initialisation failed\n";
		return false;
	}
	std::string error = Window::initAll(1024, 768);
	if (!error.empty())
	{
		std::cerr << error;
		return false;
	}

	TEST_FUNC(cr2);
	TEST_FUNC(cr3);
	TEST_FUNC(cr4);
	TEST_FUNC(cr5);
	TEST_FUNC(cr6);

	ModuleLoader::instance().preinit();
	ModuleLoader::instance().init();
	ModuleLoader::instance().clientInit();

	TEST_FUNC(cr7);

	TEST_FUNC(ci1);

	TEST_FINAL;
}

} // namespace fab
