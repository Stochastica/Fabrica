#include "test.hpp"

#include "testing.hpp"

#include "common/testInit.hpp"
#include "common/testUtil.hpp"
#ifdef FABRICA_SERVER_STANDALONE
#include "server/testServer.hpp"
#else
#include "client/testClient.hpp"
#endif

namespace fab
{

std::map<std::string, std::string> testInfo()
{
	std::map<std::string, std::string> info;

	info["0"] = "Always success";
	info["i1"] = "Module Loader";
	info["u1"] = "Chunk Loading Order";
#ifdef FABRICA_SERVER_STANDALONE
	info["sc0"] = "Dummy";
#else
	info["cr1"] = "Basic OpenGL: GLFW, GLEW, GLM";
	info["cr2"] = "The Debug Screen, class Font and Text";
	info["cr3"] = "Image Loading";
	info["cr4"] = "Displaying a texture";
	info["cr5"] = "3D, class Camera, FPS calculation";
	info["cr6"] = "RenderBlock";
	info["ci1"] = "Basic Initialisation (no graphics)";
#endif
	return info;
}

/**
 * Dummy test
 */
bool test_0()
{
	std::cout << "Test 0\n";
	return true;
}

bool test(std::string id)
{
	if (id.at(0) == 'i')
		return testInit(id);
	if (id.at(0) == 'u')
		return testUtil(id);
#ifdef FABRICA_SERVER_STANDALONE
	if (id.at(0) == 's')
		return testServer(id);
#else
	if (id.at(0) == 'c')
		return testClient(id);
#endif
	TEST_FUNC(0);
	
	TEST_FINAL;
}

} // namespace fab
