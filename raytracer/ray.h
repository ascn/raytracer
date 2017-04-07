#ifndef __RAY_H__
#define __RAY_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Ray {
public:
	Ray();
	Ray(const glm::vec3 &origin, const glm::vec3 &direction);
	Ray(const Ray &ray);

	Ray getTransformedCopy(glm::mat4 tMat) const;

	glm::vec3 origin;
	glm::vec3 direction;
};

#endif // __RAY_H__