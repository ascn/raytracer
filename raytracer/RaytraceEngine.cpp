#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>
#include <QImage>
#include <QColor>
#include <QDebug>

void RaytraceEngine::render(const Camera &camera, const Scene &scene, QImage &image,
                            uint8_t maxDepth, uint8_t samples) {

	// For each pixel, cast *samples* rays using traceRay.
	// Average the color over all samples from traceRay and set
	// color of pixel (SSAA).
	for (int i = 0; i < camera.width; ++i) {
		for (int j = 0; j < camera.height; ++j) {
			Ray ray = camera.raycast(i, j);
            glm::vec3 color = RaytraceEngine::traceRay(ray, scene, 0, maxDepth);
            //image.setPixel(i, j, qRgb(color.x, color.y, color.z));
			QRgb *line = (QRgb *) image.scanLine(j);
			line += i;
			*line = qRgb(color.x, color.y, color.z);
		}
	}
}

glm::vec3 RaytraceEngine::traceRay(const Ray &ray, const Scene &scene, 
								   uint8_t depth, uint8_t maxDepth) {

	// Recursively traces a ray up to maxDepth. If the ray hits a geometry,
	// we cast light feeler rays, add up contribution, and divide by number
	// lights. If the geometry hit is not reflective or transmissive, we
	// stop even if current recursion depth isn't max depth. Otherwise we
	// reflect/refract the ray and continue.

	return glm::vec3(255, 0, 0);
}

QImage generateAOPass(const Camera &camera, const Scene &scene,
					  int samples, float spread, float distance) {

	// Cast rays through each pixel. If the ray hits a geometry,
	// cast *samples* rays from the intersection point in a
	// cone (defined by spread, 1 is a hemisphere) oriented with
	// the normal of the intersection point. Count the number of
	// these samples that hit another geometry within *distance*.
	// Set the intensity of the pixel to be:
	// 				1 - (num_hit_samples / samples)
	// If the initial ray doesn't hit a geometry, make the pixel
	// completely transparent.
	// This resulting AO image can be blended (multiply) with
	// the rendered image to generate the desired effect.

}
