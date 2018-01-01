#ifndef FABRICA_CLIENT_DEBUGSCREEN_HPP_
#define FABRICA_CLIENT_DEBUGSCREEN_HPP_

#include "renderer/Text.hpp"
#include "Camera.hpp"

namespace fab
{

/**
 * @brief Handling the drawing of the debug screen.
 */
class DebugScreen final
{
public:
	DebugScreen(Font const* const font);

	void setWireframe(bool val) noexcept
	{
		wireframe = val;
	}

	void setFPS(float val) noexcept
	{
		fps = val;
	}

	/**
	 * @brief Loads camera data into DebugScreen.
	 */
	void setCameraData(Camera const& camera) noexcept;

	void updateGeometry();
	void draw();
private:
	Text text;

	bool wireframe;
	float fps;
	float camX, camY, camZ;
	float camYaw, camPitch;
};

// Implementations

inline void
DebugScreen::setCameraData(Camera const& camera) noexcept
{
	camX = camera.getX();
	camY = camera.getY();
	camZ = camera.getZ();

	// Convert Radians to Degrees
	camYaw = camera.getYaw() / M_PI * 180;
	camPitch = camera.getPitch() / M_PI * 180;
}

} // namespace fab

#endif // !FABRICA_CLIENT_DEBUGSCREEN_HPP_
