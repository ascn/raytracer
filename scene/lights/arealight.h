#ifndef __AREA_LIGHT_H__
#define __AREA_LIGHT_H__

#include <scene/lights/light.h>

class AreaLight : public Light {
public:
	AreaLight(Geometry *geometry);
	~AreaLight();
	glm::vec3 getCastPoint(int samples = 1, uint8_t idx = 0) const;
};

#endif // __AREA_LIGHT_H__