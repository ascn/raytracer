#ifndef __SCENE_H__
#define __SCENE_H__

#include <QList>

class Scene {
public:

	reset();

	QList<Geometry *> primitives;
	QList<Geometry *> lights;
	QList<Material *> materials;
};

#endif // __SCENE_H__