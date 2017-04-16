#include <QString>
#include <QFile>
#include <QIODevice>
#include <ostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QImage>

#include <scene/jsonreader.h>
#include <scene/scene.h>
#include <scene/camera.h>

#include <scene/geometry/cube.h>
#include <scene/geometry/sphere.h>
#include <scene/geometry/squareplane.h>
#include <scene/geometry/triangle.h>
#include <scene/geometry/mesh.h>
#include <tinyobj/tiny_obj_loader.h>

namespace jsonreader {

void readJson(Camera *camera, Scene *scene, const QString & filename)
{
    scene->reset();
      QString val;
      QFile file;

      // Read in json file, store in val
      file.setFileName(filename);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      val = file.readAll();
      file.close();
      QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

      QJsonObject object = doc.object();

      QJsonObject sceneObj = object.value("scene").toObject();
      QJsonObject cameraObj = sceneObj.value("camera").toObject();
      QJsonArray geometryArr = sceneObj.value("geometry").toArray();
      QJsonArray materialArr = sceneObj.value("material").toArray();

      parseCamera(camera, cameraObj);
      parseMaterial(scene, materialArr);
      parseGeometry(scene, geometryArr);
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
    centerArr = cameraObj.value("target").toArray();

    for (int i = 0; i < 3; i++) {
        eye[i] = eyeArr[i].toDouble();
        wUp[i] = wUpArr[i].toDouble();
        center[i] = centerArr[i].toDouble();
    }
    camera->width = w;
    camera->height = h;
    camera->fovy = fov;
    camera->ref = center;
    camera->eye = eye;
    camera->wUp = wUp;

    camera->update();
}

void parseGeometry(Scene *scene, QJsonArray geometryArr) {

    QJsonObject currObj;
    QString name;
    for (int i = 0; i < geometryArr.size(); i++) {

        currObj = geometryArr[i].toObject();
        name = currObj.value("name").toString();
        QString type = currObj.value("type").toString();

        // Create objects that go into appropriate Geometry object
        Transform *transform = new Transform();
        if (currObj.contains("transform")) {
            parseTransform(transform, currObj.value("transform").toObject());
        }

        Material *material = scene->materialsMap.value(currObj.value("material").toString());

        if (type == QString("cube")) {

            Cube *cube = new Cube(name, *transform, material);
            scene->primitives.append(cube);

        } else if (type == QString("sphere")) {

            Sphere *sphere = new Sphere(name, *transform, material);
            scene->primitives.append(sphere);
            if (material->emissive) {
                scene->lights.append(sphere);
            }
        } else if (type == QString("square")) {

            SquarePlane *sp = new SquarePlane(name, *transform, material);
            scene->primitives.append(sp);
            if (material->emissive) {
                scene->lights.append(sp);
            }

        } else if (type == QString("obj")) {
            QString filename = currObj.value("filename").toString();
            Mesh *mesh = new Mesh(name, filename, *transform, material);
            scene->primitives.append(mesh);
            if (material->emissive) {
                scene->lights.append(mesh);
            }
        }
    }
}

void parseTransform(Transform *transform, QJsonObject obj) {

    int i;
    if (obj.contains("translate")) {
        QJsonArray tl = obj.value("translate").toArray();

        for (i = 0; i < 3; i++) {
            transform->translation[i] = tl[i].toDouble();
        }
    }

    if (obj.contains("rotate")) {
        QJsonArray rt = obj.value("rotate").toArray();

        for (i = 0; i < 3; i++) {
           transform->rotation[i] = rt[i].toDouble();
        }
    }

    if (obj.contains("scale")) {
        QJsonArray sc = obj.value("scale").toArray();
        for (i = 0; i < 3; i++) {
             transform->scale[i] = sc[i].toDouble();
        }
    }

    transform->update();
}

void parseMaterial(Scene *scene, QJsonArray materialArr) {

    QJsonObject currObj;
    for (int i = 0; i < materialArr.size(); i++) {

        currObj = materialArr[i].toObject();
        Material *material = new Material();

        if (currObj.contains("type")) {

            QString type = currObj.value("type").toString();
            if (type == QString("lambert"))  {
                material->type = MaterialType(0);
            } else if (type == QString("phong")) {
                material->type = MaterialType(1);
            }
        }

        if (currObj.contains("name")) {
            material->name = currObj.value("name").toString();
        }

        if (currObj.contains("baseColor")) {
            QJsonArray bcArr = currObj.value("baseColor").toArray();
            for (int i = 0; i < 3; i++) {
                 material->baseColor[i] = bcArr[i].toDouble();
            }
        }

        if (currObj.contains("texture")) {
            QImage img(currObj.value("texture").toString());
            if (!img.isNull()) material->texture = &img;
        }

        if (currObj.contains("emissive")) {

            QString boolean = currObj.value("emissive").toString();
            if (boolean == QString("true")) {
                material->emissive = true;
            } else {
                material->emissive = false;
            }
        }

        if (currObj.contains("reflectivity")) {
            float refl = currObj.value("reflectivity").toDouble();
            material->reflectivity = refl;
        }

        if (currObj.contains("normalMap")) {
            QImage img(currObj.value("normalMap").toString());
            if (!img.isNull()) material->normalMap = &img;
        }

        if (currObj.contains("ks")) {
            material->ks = currObj.value("ks").toDouble();
        }
        if (currObj.contains("n")) {
            material->n = currObj.value("n").toInt();
        }
        if (currObj.contains("ior")) {
            material->refractive = currObj.value("ior").toDouble();
            if (currObj.value("ior").toDouble() > 0) {
                material->type = MaterialType::TRANSMISSIVE;
            }
        }

        scene->materialsMap.insert(material->name, material);
        scene->materials.append(material);
    }
}

}
