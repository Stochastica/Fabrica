#include "Client.hpp"

#include <cassert>
#include <iostream>
#include <cmath>

#include "Camera.hpp"
#include "RenderingRegistry.hpp"
#include "gui/GUIBase.hpp"
#include "renderer/utils.hpp"
#include "renderer/PerformanceMonitor.hpp"

namespace fab
{

Client::Client(ClientConfig* const config):
	living(true),
	logger(LogManager::create("CLIENT")),
	window(&Window::instance()),
	config(config),
	debugScreen(&Font::defaultFont()),

	textureManager(nullptr),
	worldRenderer(nullptr)
{
}
Client::~Client()
{
	delete textureManager;
	delete worldRenderer;
}

void Client::reloadRenderers()
{
	delete textureManager;
	textureManager = RenderingRegistry::initAll();
}
void Client::loadWorld(World* world)
{
	assert(!worldRenderer);
	logger("Loading... World");
	worldRenderer = new WorldRenderer(window,
	                                  world,
	                                  &mutexWorld,
	                                  &interactions,
	                                  textureManager);
	window->pushListener(worldRenderer);
}
void Client::execDraw()
{
	assert(textureManager);

	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	logger("Drawing loop begin");
	Camera camera;
	camera.setPosition(0.f, 20.f, 0.f);

	logger("Camera Position: [" +
	       std::to_string(camera.getX()) + ", " +
	       std::to_string(camera.getY()) + ", " +
	       std::to_string(camera.getZ()) + "]"
	      );

	PerformanceMonitor pmonitor;

	while (living)
	{
		glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		bool showDebugScreen = !!worldRenderer;

		if (worldRenderer)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);

			float const duration = pmonitor.getDuration();

			mutexWorld.lock();

			if (interactions.moveFront)
				camera.translatePara(config->naviSpeedPara * duration);
			else if (interactions.moveBack)
				camera.translatePara(-config->naviSpeedPara * duration);
			if (interactions.moveLeft)
				camera.translatePerp(config->naviSpeedPerp* duration);
			else if (interactions.moveRight)
				camera.translatePerp(-config->naviSpeedPerp* duration);
			if (interactions.moveUp)
				camera.translateVert(config->naviSpeedVert * duration);
			if (interactions.moveDown)
				camera.translateVert(-config->naviSpeedVert * duration);

			double cursorX, cursorY;
			window->getCursor(&cursorX, &cursorY);
			camera.setYawPitch((cursorX - window->getWidth() / 2) * 2 * M_PI /
			                   Window::instance().getWidth(),
			                   (cursorY - window->getHeight() / 2) * M_PI /
			                   Window::instance().getHeight());

			showDebugScreen = interactions.showDebugScreen;
			mutexWorld.unlock();

			worldRenderer->draw(camera);
		}

		glDisable(GL_DEPTH_TEST); // Planar drawing

		if (showDebugScreen)
		{
			debugScreen.setFPS(pmonitor.getFPS());
			debugScreen.setCameraData(camera);
			debugScreen.setWireframe(interactions.drawWireframe);
			debugScreen.updateGeometry();
			debugScreen.draw();
		}
		GL_ERROR_CHECK;

		window->swapBuffers();
		window->pollEvents();

		if (!window->isOpen()) living = false;
		pmonitor.update();

	}
	logger("Drawing Loop exit");
}

} // namespace fab
