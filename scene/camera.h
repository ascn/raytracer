#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <raytracer/ray.h>

class Camera {
public:
	Camera();
	Camera(uint16_t w, uint16_t h);
	Camera(uint16_t w, uint16_t h, const glm::vec3 &eye,
			const glm::vec3 &ref, const glm::vec3 &wUp);

	void lookAt(float eyeX, float eyeY, float eyeZ,
				float centerX, float centerY, float centerZ,
				float upX, float upY, float upZ);
	void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

	void lookLeft(float deg);
	void lookRight(float deg);
	void lookUp(float deg);
	void lookDown(float deg);
	void translateLeft(float x);
	void translateRight(float x);
	void translateUp(float x);
	void translateDown(float x);

	void update();
	
	Ray raycast(float x, float y) const;

	float fovy;
	uint16_t width, height;
	float near, far;
	float aspect;
	glm::vec3 eye;			// The position of the camera
	glm::vec3 center;		// The point the camera is pointing at
	glm::vec3 up;			// The up vector of the camera
	glm::vec3 right;		// The right vector of the camera
	glm::vec3 ref; 			// The forward vector of the camera
	glm::vec3 wUp;
	glm::vec3 V;
	glm::vec3 H;
	glm::mat4 proj;
	glm::mat4 view;
};

#endif // __CAMERA_H__