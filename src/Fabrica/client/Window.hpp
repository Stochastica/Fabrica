#ifndef FABRICA_CLIENT_WINDOW_HPP_
#define FABRICA_CLIENT_WINDOW_HPP_

#include <functional>
#include <stack>
#include <string>

#include <boost/algorithm/clamp.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
 *
 * The purpose of the Window singleton class is to provide a unified interface
 * for other rendering classes to access the properties of the main rendering
 * window. It therefore is necessary that the singleton is instantiated before
 * using other rendering classes.
 *
 */

namespace fab
{

/**
 * Each enum entry corresponds to a key on the keyboard. The numbers are mapped
 * to GLFW keyboard macros.
 */
enum class Key
{
	Unknown = GLFW_KEY_UNKNOWN,
	Space = GLFW_KEY_SPACE,
	Apostrophe = GLFW_KEY_APOSTROPHE, /* ' */
	Comma = GLFW_KEY_COMMA, /* , */
	Minus, /* - */
	Period, /* . */
	Slash, /* / */
	N_0 = GLFW_KEY_0, // Numbers, 0
	N_1,
	N_2,
	N_3,
	N_4,
	N_5,
	N_6,
	N_7,
	N_8,
	N_9,
	Semicolon = GLFW_KEY_SEMICOLON, /* ; */
	Equal = GLFW_KEY_EQUAL, /* = */
	A = GLFW_KEY_A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	BracketLeft = GLFW_KEY_LEFT_BRACKET, /* [ */
	Backslash, /* \ */
	BracketRight, /* ] */
	Grave = GLFW_KEY_GRAVE_ACCENT, /* ` */
	World1 = GLFW_KEY_WORLD_1,
	World2,
	Escape = GLFW_KEY_ESCAPE,
	Enter,
	Tab,
	Backspace,
	Insert,
	Delete,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,
	CapsLock = GLFW_KEY_CAPS_LOCK,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	F1 = GLFW_KEY_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,
	F25,
	KP_0 = GLFW_KEY_KP_0, // Keypad
	KP_1,
	KP_2,
	KP_3,
	KP_4,
	KP_5,
	KP_6,
	KP_7,
	KP_8,
	KP_9,
	KP_Decimal,
	KP_Divide,
	KP_Multiple,
	KP_Subtract,
	KP_Add,
	KP_Enter,
	KP_Equal,
	ShiftLeft = GLFW_KEY_LEFT_SHIFT,
	ControlLeft,
	AltLeft,
	SuperLeft,
	ShiftRight,
	ControlRight,
	AltRight,
	SuperRight,
	Menu
};
// Static assert that {@code enum class Key} agrees with GLFW
// Only the beginning and ending of contiguous blocks are checked.
#define ENUM_KEY_CHECK(entry, glfwName) \
	static_assert( (int) Key::entry == GLFW_KEY_##glfwName, \
	               "enum class Key does not match GLFW: Key::" #entry " != " \
	               "GLFW_KEY_"#glfwName)
ENUM_KEY_CHECK(Unknown, UNKNOWN);
ENUM_KEY_CHECK(Space, SPACE);
ENUM_KEY_CHECK(Apostrophe, APOSTROPHE);
ENUM_KEY_CHECK(Comma, COMMA);
ENUM_KEY_CHECK(Slash, SLASH);
ENUM_KEY_CHECK(N_0, 0);
ENUM_KEY_CHECK(N_9, 9);
ENUM_KEY_CHECK(Semicolon, SEMICOLON);
ENUM_KEY_CHECK(Equal, EQUAL);
ENUM_KEY_CHECK(A, A);
ENUM_KEY_CHECK(Z, Z);
ENUM_KEY_CHECK(BracketLeft, LEFT_BRACKET);
ENUM_KEY_CHECK(BracketRight, RIGHT_BRACKET);
ENUM_KEY_CHECK(Grave, GRAVE_ACCENT);
ENUM_KEY_CHECK(World1, WORLD_1);
ENUM_KEY_CHECK(World2, WORLD_2);
ENUM_KEY_CHECK(Escape, ESCAPE);
ENUM_KEY_CHECK(End, END);
ENUM_KEY_CHECK(CapsLock, CAPS_LOCK);
ENUM_KEY_CHECK(Pause, PAUSE);
ENUM_KEY_CHECK(F1, F1);
ENUM_KEY_CHECK(F25, F25);
ENUM_KEY_CHECK(KP_0, KP_0);
ENUM_KEY_CHECK(KP_Equal, KP_EQUAL);
ENUM_KEY_CHECK(ShiftLeft, LEFT_SHIFT);
ENUM_KEY_CHECK(Menu, MENU);
#undef ENUM_KEY_CHECK

/**
 * Key modifiers
 */
enum class KeyMod
{
	Shift = 0x1,
	Control = 0x2,
	Alt = 0x4,
	Super = 0x8
};
// Static assert that {@code enum class KeyMod} agrees with GLFW
#define ENUM_KEYMOD_CHECK(entry, glfwName) \
	static_assert( (int) KeyMod::entry == GLFW_MOD_##glfwName, \
	               "enum class KeyMod does not match GLFW: KeyMod::" #entry " != " \
	               "GLFW_MOD_"#glfwName)
ENUM_KEYMOD_CHECK(Shift, SHIFT);
ENUM_KEYMOD_CHECK(Control, CONTROL);
ENUM_KEYMOD_CHECK(Alt, ALT);
ENUM_KEYMOD_CHECK(Super, SUPER);
#undef ENUM_KEYMOD_CHECK

/**
 * Key or mouse button actions
 */
enum class KeyAction
{
	Release = 0,
	Press,
	Repeat
};
#define ENUM_KEYACTION_CHECK(entry, glfwName) \
	static_assert( (int) KeyAction::entry == GLFW_##glfwName, \
	               "enum class KeyAction does not match GLFW: KeyAction::" #entry " != " \
	               "GLFW_"#glfwName)
ENUM_KEYACTION_CHECK(Release, RELEASE);
ENUM_KEYACTION_CHECK(Press, PRESS);
ENUM_KEYACTION_CHECK(Repeat, REPEAT);
#undef ENUM_KEYACTION_CHECK

enum class MouseButton
{
	Left = 0,
	Middle,
	Right
};

/*
 *
 * Window and Window Listener classes
 *
 */
class Window;

/**
 * This interface represents an object that can receive key and cursor events.
 */
class WindowListener
{
public:
	WindowListener(Window* const w): window(w) {}
	virtual ~WindowListener() = default;

	virtual void onKey(Key, KeyAction, KeyMod) {};
	virtual void onMouse(MouseButton, bool press, KeyMod) {};

protected:
	/**
	 * @brief Pops this object from the window listener stack.
	 * @warning Only call this when the listener is on top.
	 */
	void pop();

	Window* const window;
};

/**
 * @brief Default Listener that never pops itself.
 */
class WindowListenerDefault final: public WindowListener
{
public:
	WindowListenerDefault(Window* const w):
		WindowListener(w) {}

	// Log keys
#ifndef NDEBUG
	void onKey(Key k, KeyAction ka, KeyMod km) final override;
	void onMouse(MouseButton m, bool press, KeyMod km) final override;
#endif
};

/**
 * @brief Manages the OpenGL context.
 */
class Window final
{
public:
	/**
	 * 1. Create the window (with given width and height)
	 * 2. Make the window context current
	 * 3. Initialises GLEW
	 * 4. Font::init()
	 * 5. Text::init()
	 * 6. GUIBase::init()
	 * 7. Binds the keys and size callbacks
	 *
	 * @brief Calls initialiser of GLEW and all GL-related static classes
	 * @warning Do not call this function twice.
	 * @return Emtpy string if no error occurs, otherwise the error.
	 */
	static std::string initAll(int width, int height);
	/**
	 * @brief Unloads libraries
	 */
	static void destroyAll();

	static Window& instance(); ///< Singleton instance

	bool create(int w, int h);

	bool isOpen() const { return !glfwWindowShouldClose(window); }
	void swapBuffers() { glfwSwapBuffers(window); }
	void pollEvents() { glfwPollEvents(); }
	/**
	 * Width of this window
	 */
	int getWidth() const { return width; }
	/**
	 * Height of this window
	 */
	int getHeight() const { return height; }
	float aspectRatio() const { return width / (float) height; }
	/**
	 * @brief Get the width of one pixel in GL units.
	 * @return 2/width
	 */
	float getRatioW() const { return ratioW; }
	/**
	 * @brief Get the height of one pixel in GL units.
	 * @return 2/height
	 */
	float getRatioH() const { return ratioH; }

	/**
	 * @brief Checks if the keyboard key is pressed.
	 */
	bool isKeyPressed(Key);

	/**
	 * @brief Retrieve the cursor position.
	 */
	void getCursor(double* const x, double* const y);

	/**
	 * @brief Locks the cursor to the screen and allows it to move freely.
	 */
	void cursorLock(bool lock);
	/**
	 * Modulo the cursor's x position and clamp cursor's y position so that it
	 * lies within the window.
	 *
	 * Usually used with {@code cursorLock(bool)}.
	 */
	void cursorConfine();

	/**
	 * @brief Pushes a Window Listener onto the stack.
	 * @warning Not responsible for deleting the pointers.
	 */
	void pushListener(WindowListener* l);
	/**
	 * @brief Pops a Window Listener from the stack.
	 */
	WindowListener* popListener();

private:
	Window(): listenerDefault(this) {}

	GLFWwindow* window;
	int width, height;
	float ratioW, ratioH;

	std::stack<WindowListener*> listeners;
	WindowListenerDefault listenerDefault;

	friend class WindowListener;
	/**
	 * Internal functions that are bound to GLFW
	 */
	friend void windowKeyCallback(GLFWwindow*, int, int, int, int);
	friend void windowMouseButtonCallback(GLFWwindow*, int, int, int);
	friend void windowResizeCallback(GLFWwindow*, int, int);
};

// Implementations

inline void WindowListener::pop()
{
	assert(window->listeners.top() == this &&
			"Trying to pop a listener that is not on top.");
	window->listeners.pop();
}

inline Window& Window::instance()
{
	static Window inst;
	return inst;
}

inline bool Window::isKeyPressed(Key key)
{
	return glfwGetKey(window, (int) key) == (int) KeyAction::Press;
}
inline void Window::getCursor(double* const x, double* const y)
{
	glfwGetCursorPos(window, x, y);
}
inline void Window::cursorLock(bool lock)
{
	glfwSetInputMode(window, GLFW_CURSOR,
	                 lock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
inline void Window::cursorConfine()
{
	double x,y;
	getCursor(&x, &y);
	if (x < 0) x += width;
	else if (x > width) x -= width;
	y = boost::algorithm::clamp(y, 0, (double) height);
	glfwSetCursorPos(window, x, y);
}

} // namespace fab

#endif // !FABRICA_CLIENT_WINDOW_HPP_
