#include <raytracer/RenderTask.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <raytracer/ray.h>
#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <QColor>
#include <QImage>

void RenderTask::run() {
	for (int i = minPoint.x; i < maxPoint.x; ++i) {
		for (int j = minPoint.y; j < maxPoint.y; ++j) {
			glm::vec3 total = glm::vec3(0);
			for (float k = 0; k < 1; k += 1 / (float) samples) {
                for (float l = 0; l < 1; l += 1 / (float) samples) {
					Ray ray = camera->raycast(i + k, j + l);
		            glm::vec3 color = re->traceRay(ray, *scene, 0, maxDepth);
					total += color;
				}
			}
			total /= glm::vec3(samples * samples);
			re->writeColorToImage(*img, i, j, total);
			//QRgb *line = (QRgb *) img->scanLine(j);
			//line += i;
			//*line = qRgb(total.x, total.y, total.z);
        }
	}
}