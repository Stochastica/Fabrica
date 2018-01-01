#include "ClientConfig.hpp"

namespace fab
{

void ClientConfig::read()
{
	windowWidth = 1024;
	windowHeight = 768;

	naviSpeedPara = 10.f;
	naviSpeedPerp = 5.f;
	naviSpeedVert = 8.f;

	if(!c.fileRead()) return;

	if (c.readSubtree("navigation"))
	{
		c.read(&naviSpeedPara, "Parallel_Navigation_Speed");
		c.read(&naviSpeedPerp, "Perpendicular_Navigation_Speed");
		c.read(&naviSpeedVert, "Vertical_Navigation_Speed");

		c.popSubtree();
	}
	c.popSubtree();
}
void ClientConfig::write()
{
	c.pushTree();

	c.beginSubtree();
	{
		c.write(naviSpeedPara, "Parallel_Navigation_Speed");
		c.write(naviSpeedPerp, "Perpendicular_Navigation_Speed");
		c.write(naviSpeedVert, "Vertical_Navigation_Speed");
	}
	c.endSubtree("navigation");

	c.fileWrite();
	c.popSubtree();
}

} // namespace fab
