#ifndef FABRICA_CLIENT_CLIENT_HPP_
#define FABRICA_CLIENT_CLIENT_HPP_

#include <atomic>
#include <mutex>

#include <boost/core/noncopyable.hpp>
#include <boost/filesystem.hpp>

#include "../common/LogManager.hpp"
#include "ClientConfig.hpp"
#include "DebugScreen.hpp"
#include "Window.hpp"
#include "WorldRenderer.hpp"
#include "renderer/Text.hpp"
#include "renderer/TextureManager.hpp"

namespace fab
{

/**
 * @brief Singleton class for the client.
 * 
 * Currently not functioning properly. More test needed.
 */
class Client final: boost::noncopyable
{
public:
	Client(ClientConfig* const config);
	~Client();

	void halt() { living = false; }

	void reloadRenderers();
	/**
	 * @warning Must not be called when a world is already loaded.
	 */
	void loadWorld(World* const);

	/**
	 * @brief The main rendering loop
	 */
	void execDraw();

private:
	std::atomic_bool living;

	Logger logger;
	Window* window;
	ClientConfig* config;
	DebugScreen debugScreen;

	// Client holds onto ownership of these objects
	TextureManager* textureManager;
	WorldRenderer* worldRenderer;

	std::mutex mutexWorld;
	InteractionFlags interactions;
};

} // namespace fab

#endif // !FABRICA_CLIENT_CLIENT_HPP_
