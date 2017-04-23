#ifndef __RAYTRACEENGINE_H__
#define __RAYTRACEENGINE_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>
#include <QImage>
#include <QObject>

class RaytraceEngine : public QObject {
	Q_OBJECT
public:
	RaytraceEngine();

	void render(const Camera &camera, const Scene &scene, QImage &image, 
		uint8_t maxDepth = 5, uint8_t samples = 1, bool multithreading = true);
	glm::vec3 traceRay(const Ray &ray, const Scene &scene,
		uint8_t depth, uint8_t maxDepth, uint8_t samples = 1, uint8_t idx = 0);
	QImage generateAOPass(const Camera &camera, const Scene &scene,
		int samples, float spread, float distance);
	glm::vec4 traceAORay(const Ray &ray, const Scene &scene,
		int samples, float spread, float distance);
	float fresnel(const Ray &ray, glm::vec3 normal, float ior);
	void writeColorToImage(QImage &img, int x, int y, glm::vec3 color);
	void emitUpdateGUI();

signals:
	void updateGUI();
};

#endif // __RAYTRACEENGINE_H__
