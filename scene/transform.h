#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform {
public:
	glm::mat4 transMat();
	glm::mat4 invTransMat();

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 transform;
	glm::mat4 invTransform;
};

#endif // __TRANSFORM_H__