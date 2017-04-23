#include <scene/lights/arealight.h>
#include <sampler/sampler.h>

AreaLight::AreaLight(Geometry *geometry) {
	this->geometry = geometry;
}

AreaLight::~AreaLight() {}

glm::vec3 AreaLight::getCastPoint(int samples, uint8_t idx) const {
	Sampler sampler;
	std::vector<glm::vec3> points;
	sampler.generateSamples(samples, points, Warp::Square);
	// Map points to the geometry's transform
	for (auto &p : points) {
		p = geometry->transform.transform * glm::vec4(p.x - 0.5, p.y - 0.5, 0, 1);
	}
	return points[idx];
}