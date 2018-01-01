#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace fab
{

glm::mat4 Camera::matrix(float aspectRatio) const noexcept
{
	glm::mat4 mProjection = glm::perspective(glm::radians(45.f),
	                        aspectRatio, .1f, 100.f);
	// World space to View space
	glm::mat4 mView = glm::eulerAngleXY(pitch, yaw);
	glm::mat4 mTranslate = glm::translate(glm::mat4(1.f), -glm::vec3(position));

	return mProjection * mView * mTranslate;
}

} // namespace fab
