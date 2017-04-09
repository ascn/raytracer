#ifndef __RAYTRACEENGINE_H__
#define __RAYTRACEENGINE_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>

class RaytraceEngine {
	static void render(const Camera &camera, const Scene &scene, QImage &image, 
		uint8_t recursionDepth = 5, uint8_t samples = 1);
	static glm::vec3 traceRay(const Ray &ray, uint8_t depth, uint8_t maxDepth);
}

#endif // __RAYTRACEENGINE_H__