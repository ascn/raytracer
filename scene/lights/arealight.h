#ifndef __AREA_LIGHT_H__
#define __AREA_LIGHT_H__

#include <scene/lights/light.h>

class AreaLight : public Light {
public:
	AreaLight(Geometry *geometry);
	~AreaLight();
	glm::vec3 getCastPoint(int samples, uint8_t idx) const;
};

#endif // __AREA_LIGHT_H__