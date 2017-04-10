#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <raytracer/intersection.h>
#include <scene/geometry/geometry.h>
#include <raytracer/ray.h>
#include <scene/scene.h>

Intersection::Intersection() :
	isectPoint(glm::vec3(0, 0, 0)), normal(glm::vec3(0, 0, 0)),
	objectHit(nullptr), t(-1) {}

Intersection getIntersection(const Ray &ray, const Scene &scene) {
	// Iterate through the geometries in the scene for the given ray
	// and calculate the intersection for each. Update if the t value
	// for a new intersection is less than the current.
}