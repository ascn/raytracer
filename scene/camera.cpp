#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <raytracer/ray.h>
#include <scene/camera.h>

Camera::Camera() : Camera(512, 512) {}

Camera::Camera(uint16_t w, uint16_t h) :
	Camera(w, h, glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) {}

Camera::Camera(uint16_t w, uint16_t h, const glm::vec3 &eye,
	const glm::vec3 &ref, const glm::vec3 &wUp) :
	fovy(45), width(w), height(h), near(0.01), far(1000),
	eye(eye), ref(ref), wUp(wUp)
{
	update();
}

void Camera::lookAt(float eyeX, float eyeY, float eyeZ,
					float centerX, float centerY, float centerZ,
					float upX, float upY, float upZ) {
	this->eye = glm::vec3(eyeX, eyeY, eyeZ);
	this->center = glm::vec3(centerX, centerY, centerZ);
	this->up = glm::vec3(upX, upY, upZ);
	update();
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
	this->eye = eye;
	this->center = center;
	this->up = up;
	update();
}

void Camera::update() {
	center = glm::normalize(ref - eye);
	right = glm::normalize(glm::cross(center, wUp));
	up = glm::cross(right, center);
	aspect = (float) width / height;
	float tan_alpha = glm::tan(fovy / 2);
	float len = glm::length(ref - eye);
	V = up * len * tan_alpha;
	H = right * len * aspect * tan_alpha;
}

Ray Camera::raycast(float x, float y) const {
	float sx = (2.0f * x / width - 1);
	float sy = (1 - 2.0f * y / height);
	glm::vec3 p = ref + sx * H + sy * V;
	return Ray(eye, glm::normalize(p - eye));
}