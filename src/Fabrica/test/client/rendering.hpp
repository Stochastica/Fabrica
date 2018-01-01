#ifndef FABRICA_TEST_CLIENT_RENDERING_HPP_
#define FABRICA_TEST_CLIENT_RENDERING_HPP_

namespace fab
{

/**
 * Test cr1:
 *  Test the fundamental functionalities of OpenGL. It directly accesses the
 *  OpenGL interfaces.
 *
 *  Objectives: Test Functionality of GLFW
 *              Query OpenGL parameters.
 *              Create a Window with a grey triangle in it.
 *
 */
bool test_cr1();
/**
 * Test cr2:
 *  Tests the {@code Window, Font, Text, DebugScreen} classes.
 *
 *  Objective: Display the debug screen
 *             Test functionality of the Text class
 *
 */
bool test_cr2();
/**
 * Test cr3:
 *  Tests the ResourceLocation resolving and image loading mechanism.
 *
 *  Objective: Print a 16x16 grid of characters on screen.
 */
bool test_cr3();
/**
 * Test cr4:
 *	Tests the {@code TextureAtlas} class, and texture loading mechanisms.
 *
 *	Objective: Display the grass texture on 1/4 of the window.
 */
bool test_cr4();
/**
 * Test cr5:
 *	Tests the {@code Camera} class and 3D drawing
 *
 *	Objective: Display a grass block.
 *	           Allow for rotating the camera.
 *	           Test FPS mechanisms
 */
bool test_cr5();

/**
 * Test cr6:
 *	Identical functionality as cr5, but achieved using RenderBlocks. It also
 *	makes sure that facing flags function properly.
 */
bool test_cr6();

/**
 * Test cr7:
 *	The WorldRenderer class.
 */
bool test_cr7();

} // namespace fab

#endif // !FABRICA_TEST_CLIENT_RENDERING_HPP_
