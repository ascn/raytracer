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

Ray Ray::getTransformedCopy(glm::mat4 tMat) const {
	Ray ret;
	glm::vec4 trans_orig = tMat * glm::vec4(this->origin, 1);
	glm::vec4 trans_dir = tMat * glm::vec4(this->direction, 1);
	ret.origin.x = trans_orig.x;
	ret.origin.y = trans_orig.y;
	ret.origin.z = trans_orig.z;
	ret.direction.x = trans_dir.x;
	ret.direction.y = trans_dir.y;
	ret.direction.z = trans_dir.z;
	return ret;
}