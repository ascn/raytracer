#include <QString>
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>


#include <scene/geometry/cube.h>
#include <scene/geometry/sphere.h>
#include <scene/geometry/squareplane.h>
#include <scene/geometry/triangle.h>
#include <scene/transform.h>
#include <scene/camera.h>
#include <scene/scene.h>

namespace jsonreader {
    void readJson(Camera *camera, Scene *scene, const QString & filename);
    void parseCamera(Camera *camera, QJsonObject cameraObj);
    void parseGeometry(Scene *scene, QJsonArray geometry);
    void parseMaterial(Scene *scene, QJsonArray material);
    void parseTransform(Transform *transform, QJsonObject obj);
}
