#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <raytracer/ray.h>

Ray::Ray() {
	this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
	this->direction = glm::vec3(1.0f, 0.0f, 0.0f);
}

Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction) {
	this->origin = origin;
	this->direction = direction;
}

Ray Ray::getTransformedCopy(const glm::mat4 tMat) const {
	glm::vec4 trans_orig = tMat * glm::vec4(this->origin, 1);
	glm::vec4 trans_dir = tMat * glm::vec4(this->direction, 0);
	return Ray(glm::vec3(trans_orig.x, trans_orig.y, trans_orig.z),
				glm::vec3(trans_dir.x, trans_dir.y, trans_dir.z));
}
