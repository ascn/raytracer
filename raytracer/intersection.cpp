#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <raytracer/intersection.h>
#include <scene/geometry/geometry.h>
#include <raytracer/ray.h>
#include <scene/scene.h>

Intersection::Intersection() :
	isectPoint(glm::vec3(0, 0, 0)), normal(glm::vec3(0, 0, 0)),
	objectHit(nullptr), t(INFINITY) {}

Intersection::~Intersection() {}

Ray Intersection::raycast(glm::vec3 point) {
	return Ray(this->isectPoint + glm::vec3(0.0001) * this->normal,
				point - this->isectPoint);
}

Intersection Intersection::getIntersection(const Ray &ray, const Scene &scene) {
	// Iterate through the geometries in the scene for the given ray
	// and calculate the intersection for each. Update if the t value
	// for a new intersection is less than the current.
	if (scene.kdtree != nullptr) {
		Intersection ret;
		scene.kdtree->intersect(ray, &ret);
        return ret;
	}
	Intersection ret, tmp;

	for (auto &p : scene.primitives) {
		if (p->intersect(ray, &tmp)) {
			if (ret.t < 0) { ret = tmp; }
			else if (tmp.t > 0 && tmp.t < ret.t) {
				ret = tmp;
			}
		}
	}

	return ret;
}
