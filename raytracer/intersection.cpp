#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <raytracer/intersection.h>
#include <scene/geometry/geometry.h>

Intersection::Intersection() :
	isectPoint(glm::vec3(0, 0, 0)), normal(glm::vec3(0, 0, 0)),
	objectHit(nullptr), t(-1) {}