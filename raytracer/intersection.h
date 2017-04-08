#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <scene/geometry/geometry.h>

class Intersection {
public:
	Intersection();
	~Intersection();

	glm::vec3 isectPoint;
	glm::vec3 normal;
	Geometry const *objectHit;
	float t;
};

#endif // __INTERSECTION_H__