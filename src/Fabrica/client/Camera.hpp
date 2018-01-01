#ifndef FABRICA_CLIENT_CAMERA_HPP_
#define FABRICA_CLIENT_CAMERA_HPP_

#include <boost/algorithm/clamp.hpp>
#include <glm/glm.hpp>

#include "../util/vector.hpp"

namespace fab
{

/**
 * @brief A navigatable camera for debugging purposes
 */
class Camera
{
public:
	Camera() noexcept: yaw(0.f), pitch(0.f), position(0.f,0.f,-5.f) {}

	void setPosition(float x, float y, float z) noexcept
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	/**
	 * Move parallel to facing
	 */
	void translatePara(float p) noexcept
	{
		position.x += p * std::sin(yaw);
		position.z -= p * std::cos(yaw);
	}
	/**
	 * Move perpendicular to facing
	 */
	void translatePerp(float p) noexcept
	{
		position.x += p * std::cos(yaw);
		position.z += p * std::sin(yaw);
	}
	/**
	 * Move up/down
	 */
	void translateVert(float p) noexcept
	{
		position.y += p;
	}

	void setYawPitch(float y, float p) noexcept
	{
		yaw = y;
		pitch = p;
	}

	glm::mat4 matrix(float aspectRatio) const noexcept;

	float getX() const noexcept { return position.x; }
	float getY() const noexcept { return position.y; }
	float getZ() const noexcept { return position.z; }
	float getYaw() const noexcept { return yaw; }
	float getPitch() const noexcept { return pitch; }
private:
	float yaw, pitch;
	glm::vec3 position;
};

} // namespace fab

#endif // !FABRICA_CLIENT_CAMERA_HPP_
