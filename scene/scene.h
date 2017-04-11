#ifndef __SCENE_H__
#define __SCENE_H__

#include <QList>
#include <QMap>
#include <scene/geometry/geometry.h>
#include <scene/geometry/sphere.h>
#include <scene/camera.h>

class Scene {
public:

    void reset();

    QList<Geometry *> primitives;
    QList<Geometry *> lights;
    QList<Material *> materials;
    QMap<QString, Material *> materialsMap;

};

#endif // __SCENE_H__
