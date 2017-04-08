#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

class Transform {
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};

#endif // __TRANSFORM_H__