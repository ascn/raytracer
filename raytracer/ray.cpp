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
	ret.origin = this->origin;
	glm::vec4 trans = tMat * glm::vec4(this->direction, 1);
	ret.direction.x = trans.x;
	ret.direction.y = trans.y;
	ret.direction.z = trans.z;
	return ret;
}