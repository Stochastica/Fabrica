#include "DebugScreen.hpp"

#include <sstream>
#include <iomanip>

#include "Window.hpp"
#include "renderer/utils.hpp"

namespace fab
{

DebugScreen::DebugScreen(Font const* const font):
	text(font), wireframe(false),

	fps(0.f), camX(0.f), camY(0.f), camZ(0.f),
	camYaw(0.f), camPitch(0.f)
{
}

void DebugScreen::updateGeometry()
{
	std::stringstream string;
	string << std::fixed << std::setprecision(1);

	// Window
	string << "Window: [" << Window::instance().getWidth()
	       << ", " << Window::instance().getHeight() << "]";
	if (wireframe) string << " in Wireframe";

	// FPS
	string << " FPS: " << fps;
	
	// Camera
	string << "\nCamera: [" << camX << ", " << camY << ", " << camZ
	       << "], [" << camYaw << ", " << camPitch << "]";

	text.setContents(-1.f, 1.f, Text::TOP_LEFT, string.str());
}
void DebugScreen::draw()
{
	text.draw();
}

} // namespace fab
