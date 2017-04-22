#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <raytracer/ray.h>

class Triangle;

enum class Axis { X, Y, Z };

class BoundingBox {
public:
	BoundingBox() : minPoint(glm::vec3(0)), maxPoint(glm::vec3(0)) {}
	bool intersect(const Ray &ray, float &t) const;
	static BoundingBox Union(const BoundingBox &bb1, const BoundingBox &bb2);
	Axis getLongestAxis() const;
	glm::vec3 getMidpoint() const;

	glm::vec3 minPoint;
	glm::vec3 maxPoint;
};

BoundingBox getAABB(std::vector<Triangle *> &primitives);

#endif // __BOUNDING_BOX_H__