#include <raytracer/RenderTask.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <raytracer/ray.h>
#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <QColor>
#include <QImage>
#include <sampler/sampler.h>

void RenderTask::run() {
	Sampler sampler;
	for (int i = minPoint.x; i < maxPoint.x; ++i) {
		for (int j = minPoint.y; j < maxPoint.y; ++j) {
			glm::vec3 total = glm::vec3(0);
			std::vector<glm::vec3> points;
			sampler.generateSamples(samples, points, Warp::Square);
			for (int k = 0; k < points.size(); ++k) {
				glm::vec3 v = points[k];
				Ray ray = camera->raycast(i + v.x, j + v.y);
				glm::vec3 color = re->traceRay(ray, *scene, 0, maxDepth, samples, k);
				total += color;
			}
			total /= glm::vec3(samples);
            img.setPixel(i, j, qRgb(total.x, total.y, total.z));
            //QRgb *line = (QRgb *) img->scanLine(j);
            //line += i;
            //*line = qRgb(total.x, total.y, total.z);
        }
	}
}
