#include "Window.hpp"

#include <iostream>
#include <boost/algorithm/clamp.hpp>

#include "../common/fabrica.hpp"
#include "WorldRenderer.hpp"
#include "gui/GUIBase.hpp"
#include "renderer/Text.hpp"

namespace fab
{

#ifndef NDEBUG
void WindowListenerDefault::onKey(Key k,
                                  KeyAction ka,
                                  KeyMod km)
{
	if (ka == KeyAction::Release) return; // Release not logged.
	std::cout << "Key Pressed: ["
	          << (int) k << ", "
	          << (int) ka << ", "
	          << (int) km << "]\n";
}
void WindowListenerDefault::onMouse(MouseButton m, bool press, KeyMod km)
{
	if (!press) return;

	double cursorX, cursorY;
	window->getCursor(&cursorX, &cursorY);
	std::cout << "Mouse Button: [";
	switch (m)
	{
	case MouseButton::Left:
		std::cout << "L, ";
		break;
	case MouseButton::Right:
		std::cout << "R, ";
		break;
	case MouseButton::Middle:
		std::cout << "M, ";
		break;
	}
	std::cout << (int) km << "] at ["
	          << cursorX << ", " << cursorY << "]\n";
}
#endif

// Callbacks
void windowKeyCallback(GLFWwindow* window,
                       int key, int scancode, int action, int mods)
{
	assert(window == Window::instance().window &&
	       "client/Window.cpp: GLFW Window does not match"
	       "class Window singleton.");
	(void) window;
	(void) scancode;
	// We will not make an attempt to check that the stack is
	// non-empty. It is assumed to be always non-empty.
	Window::instance().listeners.top()->onKey(
	  (Key) key,
	  (KeyAction) action,
	  (KeyMod) mods);
}
void windowMouseButtonCallback(GLFWwindow* window,
                               int button, int action, int mods)
{
	assert(window == Window::instance().window &&
	       "client/Window.cpp: GLFW Window does not match"
	       "class Window singleton.");
	(void) window;
	bool press = action == GLFW_PRESS;
	WindowListener* const listener = Window::instance().listeners.top();
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		listener->onMouse(MouseButton::Left, press, (KeyMod) mods);
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		listener->onMouse(MouseButton::Right, press, (KeyMod) mods);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		listener->onMouse(MouseButton::Middle,press, (KeyMod) mods);
		break;
	default: // Do nothing.
		break;
	}
}
void windowResizeCallback(GLFWwindow* window,
                          int width, int height)
{
	assert(window == Window::instance().window &&
	       "client/Window.cpp: GLFW Window does not match"
	       "class Window singleton.");
	(void) window;
	Window::instance().width = width;
	Window::instance().height = height;
	Window::instance().ratioW = 2.f / width;
	Window::instance().ratioH = 2.f / height;
	glViewport(0, 0, width, height);
}

std::string Window::initAll(int width, int height)
{
	if (!instance().create(width, height))
		return "class Window: Window or OpenGL context creation failed";
	glfwMakeContextCurrent(instance().window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		return "class Window: GLEW initialisation failed";

	if (!Font::init())
		return "class Window: Font initialisation failed";

	Text::init();
	GUIBase::init();
	WorldRenderer::init();

	// Binds all callbacks
	glfwSetKeyCallback(instance().window, &windowKeyCallback);
	glfwSetMouseButtonCallback(instance().window, &windowMouseButtonCallback);
	glfwSetWindowSizeCallback(instance().window, &windowResizeCallback);

	return std::string();
}

bool Window::create(int w, int h)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(w, h, FABRICA_NAME, nullptr, nullptr);
	if (!window) return false;

	// Minimum size allowed is 300 x 200
	glfwSetWindowSizeLimits(window,
			300, 200,
			GLFW_DONT_CARE, GLFW_DONT_CARE);

	width = w;
	height = h;
	ratioW = 2.f / w;
	ratioH = 2.f / h;

	glViewport(0, 0, w, h);

	pushListener(&listenerDefault);

	return true;
}
void Window::destroyAll()
{
	glfwTerminate();
}

void Window::pushListener(WindowListener* l)
{
	listeners.push(l);
}
WindowListener* Window::popListener()
{
	if (listeners.empty())
	{
		assert(false && "class Window: Trying to pop empty listener stack.");
		return nullptr;
	}
	auto temp = listeners.top();
	listeners.pop();
	return temp;
}


} // namespace fab
