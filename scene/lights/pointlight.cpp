#include <scene/lights/pointlight.h>

PointLight::PointLight(Geometry *geometry) {
	this->geometry = geometry;
}

PointLight::~PointLight() {}

glm::vec3 PointLight::getCastPoint(int samples, uint8_t idx) const {
    (void) samples; (void) idx;
	return geometry->transform.translation;
}
