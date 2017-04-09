#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>
#include <QImage>

void RaytraceEngine::render(const Camera &camera, const Scene &scene, QImage &image,
							uint8_t maxDepth = 5, uint8_t samples = 1) {

	// For each pixel, cast *samples* rays using traceRay.
	// Average the color over all samples from traceRay and set
	// color of pixel.

	Ray ray;

	for (int i = 0; i < camera.width; ++i) {
		for (int j = 0; j < camera.height; ++j) {

		}
	}

}

glm::vec3 RaytraceEngine::traceRay(const Ray &ray, uint8_t depth, uint8_t maxDepth) {

	// Recursively traces a ray up to maxDepth. If the ray hits a geometry,
	// we cast light feeler rays, add up contribution, and divide by number
	// lights. If the geometry hit is not reflective or transmissive, we
	// stop even if current recursion depth isn't max depth. Otherwise we
	// reflect/refract the ray and continue.

}