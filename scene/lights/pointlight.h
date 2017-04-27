#ifndef __POINT_LIGHT_H__
#define __POINT_LIGHT_H__

#include <scene/lights/light.h>

class PointLight : public Light {
public:
	PointLight(Geometry *geometry);
	~PointLight();
	glm::vec3 getCastPoint(int samples = 1, uint8_t idx = 0) const;
};

#endif // __POINT_LIGHT_H__