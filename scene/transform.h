#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform {
public:
	Transform();
	Transform(glm::vec3 t, glm::vec3 r, glm::vec3 s);

	void update();

	const glm::mat4 &transMat() { return transform; }
	const glm::mat4 &invTransMat() { return invTransform; }

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 transform;
	glm::mat4 invTransform;
	glm::mat4 invTransTrans;
};

#endif // __TRANSFORM_H__