#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <raytracer/ray.h>

class Geometry;
class Scene;

class Intersection {
public:
	Intersection();
	~Intersection();

	static Intersection getIntersection(const Ray &ray, const Scene &scene);

	glm::vec3 isectPoint;
	glm::vec3 normal;
	Geometry const *objectHit;
	float t;
};

#endif // __INTERSECTION_H__
