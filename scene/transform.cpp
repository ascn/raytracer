#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <scene/transform.h>

Transform::Transform() :
	translation(glm::vec3(0)),
	rotation(glm::vec3(0)),
	scale(glm::vec3(1)) { update(); }

Transform::Transform(glm::vec3 t, glm::vec3 r, glm::vec3 s) :
	translation(t), rotation(r), scale(s) { update(); }

void Transform::update() {
	transform = glm::translate(glm::mat4(1.0f), translation) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), scale);
	invTransform = glm::inverse(transform);
}