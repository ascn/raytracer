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
#include <scene/geometry/cylinder.h>
#include <scene/geometry/cone.h>
#include <scene/geometry/disk.h>
#include <scene/geometry/hyperboloid.h>
#include <scene/geometry/paraboloid.h>
#include <scene/lights/arealight.h>
#include <scene/lights/pointlight.h>

namespace jsonreader {

void readJson(Camera *camera, Scene *scene, const QString & filename, bool kd)
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
      parseGeometry(scene, geometryArr, kd);
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

void parseGeometry(Scene *scene, QJsonArray geometryArr, bool kd) {

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
                PointLight *pl = new PointLight(sphere);
                scene->lights.append(pl);
            }
        } else if (type == QString("square")) {

            SquarePlane *sp = new SquarePlane(name, *transform, material);
            scene->primitives.append(sp);
            if (material->emissive) {
                AreaLight *al = new AreaLight(sp);
                scene->lights.append(al);
            }

        } else if (type == QString("obj")) {
            QString filename = currObj.value("filename").toString();
            bool flip = false;
            if (currObj.contains("flipNormal")) {
                if (currObj.value("flipNormal").toString() == "true") {
                    flip = true;
                }
            }
            Mesh *mesh = new Mesh(name, filename, *transform, material, kd, flip);
            scene->primitives.append(mesh);
        } else if (type == QString("cylinder")) {
            Cylinder *cyl = new Cylinder(name, *transform, material);
            scene->primitives.append(cyl);
        } else if (type == QString("cone")) {
            Cone *cone = new Cone(name, *transform, material);
            scene->primitives.append(cone);
        } else if (type == QString("disk")) {
            Disk *disk = new Disk(name, *transform, material);
            scene->primitives.append(disk);
        } else if (type == QString("hyperboloid")) {
            Hyperboloid *hyper = new Hyperboloid(name, *transform, material);
            scene->primitives.append(hyper);
        } else if (type == QString("paraboloid")) {
            Paraboloid *para = new Paraboloid(name, *transform, material);
            scene->primitives.append(para);
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
            if (!img.isNull()) material->texture = new QImage(currObj.value("texture").toString());
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
            if (!img.isNull()) material->normalMap = new QImage(currObj.value("normalMap").toString());
        }

        if (currObj.contains("ks")) {
            material->ks = currObj.value("ks").toDouble();
        }
        if (currObj.contains("n")) {
            material->n = currObj.value("n").toInt();
        }
        if (currObj.contains("iorIn")) {
            material->iorIn = currObj.value("iorIn").toDouble();
        }
        if (currObj.contains("iorOut")) {
            material->iorOut = currObj.value("iorOut").toDouble();
        }
        if (currObj.value("iorOut").toDouble() > 0 &&
                currObj.value("iorIn").toDouble() > 0) {
            material->type = MaterialType::TRANSMISSIVE;
        }

        scene->materialsMap.insert(material->name, material);
        scene->materials.append(material);
    }
}

}
