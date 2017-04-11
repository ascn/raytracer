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


namespace jsonreader {

void readJson(const QString & filename)
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

      QJsonObject scene = object.value("scene").toObject();
      QJsonObject camera = scene.value("camera").toObject();
      QJsonArray geometry = scene.value("geometry").toArray();
      QJsonArray material = scene.value("material").toArray();

      parseCamera(camera);
      parseGeometry(geometry);
      parseMaterial(material);
}

void parseCamera(QJsonObject camera) {

    std::cout << "parse camera called!!" << std::endl;

    uint16_t w, h;
    QJsonArray eyeArr, wUpArr, centerArr;
    glm::vec3 eye, wUp, center;

    float fov = cameraObj.value("fov").toDouble();
    w = camera.value("width").toInt();
    h = camera.value("height").toInt();

    eyeArr = camera.value("eye").toArray();
    wUpArr = camera.value("worldUp").toArray();
    centerArr = camera.value("center").toArray();

    for (int i = 0; i < 3; i++) {
        eye[i] = eyeArr[i].toDouble();
        wUp[i] = wUpArr[i].toDouble();
        center[i] = centerArr[i].toDouble();
    }

    Camera::Camera c(w, h, eye, center, wUp);
}

void parseGeometry(QJsonArray geometry) {

    for (int i = 0; i < geometry.size(); i++) {

        String &type = geometry[i].value("type").toString();

        if ((name, "cube") == 0)  {

        } else if ((name, "square") == 0 ) {

        }  else if ((name, "obj") == 0 ) {

        }  else if ((name, "triangle") == 0 ) {

        }

    }
}

void parseMaterial(QJsonArray material) {


}

//      if (doc.isNull()) {
//          std::cout << "Invalid JSON document!" << std::endl;
////        qDebug() << "Invalid JSON document!";
//      } else {
//        std::cout << "JSON Document is properly formatted!" << std::endl;

////        innerObjArray = object["scene"].toArray();
////        foreach (const QJsonValue & value, innerObjArray) {
////            std::cout << "hey" << std::endl;
////        }

////        std::cout << "count:" << innerObjArray["geometry"].toString() << std::endl;
//      }


//      foreach (const QJsonValue & value, innerobj) {
//          std::cout << "hello" << std::endl;
//          QJsonObject obj = value.toObject();
//          std::cout << obj["camera"].toString().toStdString() << std::endl;
////          propertyNames.append(obj["PropertyName"].toString());
////          propertyKeys.append(obj["key"].toString());
//      }


//    std::cout << camera["target"].toString().toStdString() << std::endl;

//    QJsonArray jsonArray = jsonObject["scene"].toArray();


//      QJsonValue value = sett2.value(QString("scene"));
//      QJsonObject item = value.toObject();
//      std::cout << "hey" << std::endl;
//      std::cout << value.toString().toStdString() << std::endl;

      /* in case of string value get value and convert into string*/
//      qWarning() << tr("QJsonObject[appName] of description: ") << item["description"];
//      QJsonValue subobj = item["description"];
//      qWarning() << subobj.toString();

//      /* in case of array get array and convert into string*/
//      qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
//      QJsonArray test = item["imp"].toArray();
//      qWarning() << test[1].toString();



//QStringList propertyNames;
//QStringList propertyKeys;
//QString strReply = (QString)reply->readAll();
//QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

//QJsonObject jsonObject = jsonResponse.object();
//QJsonArray jsonArray = jsonObject["properties"].toArray();

}
