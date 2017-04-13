#ifndef __RAYTRACEENGINE_H__
#define __RAYTRACEENGINE_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>
#include <QImage>

class RaytraceEngine {
public:
	static void render(const Camera &camera, const Scene &scene, QImage &image, 
		uint8_t maxDepth = 5, uint8_t samples = 1, bool multithreading = true);
	static glm::vec3 traceRay(const Ray &ray, const Scene &scene,
		uint8_t depth, uint8_t maxDepth);
	static QImage generateAOPass(const Camera &camera, const Scene &scene,
		int samples, float spread, float distance);
	static glm::vec4 traceAORay(const Ray &ray, const Scene &scene,
		int samples, float spread, float distance);
};

#endif // __RAYTRACEENGINE_H__
