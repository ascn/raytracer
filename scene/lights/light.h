#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/geometry/geometry.h>

class Light {
public:
	virtual ~Light() {}
	virtual glm::vec3 getCastPoint(int samples, uint8_t idx) const = 0;
	Geometry *geometry;
};

#endif // __LIGHT_H__