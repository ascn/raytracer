#include <QString>
#include <QFile>
#include <QIODevice>
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>

#include <scene/camera.h>
#include <scene/scene.h>

namespace jsonreader {
    void readJson(Camera *camera, Scene *scene, const QString & filename);
    void parseCamera(Camera *camera, QJsonObject cameraObj);
    void parseGeometry(Scene *scene, QJsonArray geometry);
    void parseMaterial(Scene *scene, QJsonArray material);
}
