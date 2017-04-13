#ifndef __RENDERTASK_H__
#define __RENDERTASK_H__

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <QRunnable>
#include <scene/camera.h>
#include <scene/scene.h>
#include <QImage>

class RenderTask : public QRunnable {
public:
	RenderTask(glm::vec2 minPoint, glm::vec2 maxPoint, const Camera *c, const Scene *s,
		QImage *img, uint8_t maxDepth, uint8_t samples) :
		minPoint(minPoint), maxPoint(maxPoint), camera(c), scene(s),
		img(img), maxDepth(maxDepth), samples(samples) {}

	void run();

	glm::vec2 minPoint;
	glm::vec2 maxPoint;
	const Camera *camera;
	const Scene *scene;
	QImage *img;
	uint8_t maxDepth;
	uint8_t samples;
};

#endif // __RENDERTASK_H__