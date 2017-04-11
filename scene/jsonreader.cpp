#include <QString>
#include <QFile>
#include <QIODevice>
#include <ostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include <scene/jsonreader.h>
#include <scene/scene.h>
#include <scene/camera.h>

#include <scene/geometry/cube.h>
#include <scene/geometry/sphere.h>
#include <scene/geometry/squareplane.h>
#include <scene/geometry/triangle.h>

namespace jsonreader {

void readJson(Camera *camera, Scene *scene, const QString & filename)
{
      QString val;
      QFile file;

      // Read in json file, store in val
      file.setFileName(filename);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      val = file.readAll();
      file.close();
      QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

      qDebug() << doc;

      QJsonObject object = doc.object();

      QJsonObject sceneObj = object.value("scene").toObject();
      QJsonObject cameraObj = sceneObj.value("camera").toObject();
      QJsonArray geometryArr = sceneObj.value("geometry").toArray();
      QJsonArray materialArr = sceneObj.value("material").toArray();

      parseCamera(camera, cameraObj);
      parseGeometry(scene, geometryArr);
      parseMaterial(scene, materialArr);
}

void parseCamera(Camera *camera, QJsonObject cameraObj) {

    uint16_t w, h;
    QJsonArray eyeArr, wUpArr, centerArr;
    glm::vec3 eye, wUp, center;

    float fov = cameraObj.value("fov").toDouble();
    w = cameraObj.value("width").toInt();
    h = cameraObj.value("height").toInt();

    eyeArr = cameraObj.value("eye").toArray();
    wUpArr = cameraObj.value("worldUp").toArray();
    centerArr = cameraObj.value("center").toArray();

    for (int i = 0; i < 3; i++) {
        eye[i] = eyeArr[i].toDouble();
        wUp[i] = wUpArr[i].toDouble();
        center[i] = centerArr[i].toDouble();
    }

    camera->width = w;
    camera->height = h;
    camera->fovy = fov;
    camera->center = center;
    camera->eye = eye;
    camera->up = wUp;

    camera->update();
}

void parseGeometry(Scene *scene, QJsonArray geometry) {

}

void parseMaterial(Scene *scene, QJsonArray material) {

}

}
