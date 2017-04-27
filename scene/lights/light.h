#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/geometry/geometry.h>

class Light {
public:
	virtual ~Light() {}
	virtual glm::vec3 getCastPoint(int samples = 1, uint8_t idx = 0) const = 0;
	Geometry *geometry;
};

#endif // __LIGHT_H__