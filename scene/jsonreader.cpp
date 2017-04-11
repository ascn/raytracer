/* test.json */
//{
//   "appDesc": {
//      "description": "SomeDescription",
//      "message": "SomeMessage"
//   },
//   "appName": {
//      "description": "Home",
//      "message": "Welcome",
//      "imp":["awesome","best","good"]
//   }
//}

#include <QString>
#include <QFile>
#include <QIODevice>
#include <ostream>
#include <QJsonDocument>
#include <QJsonObject>

#include <scene/jsonreader.h>
#include <scene/scene.h>

void jsonreader::readJson(const QString & filename)
{
      QString val;
      QFile file;

      // Read in json file, store in val
      file.setFileName(filename);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      val = file.readAll();
      file.close();

      QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
      QJsonObject sett2 = d.object();
      QJsonValue value = sett2.value(QString("scene"));
      QJsonObject item = value.toObject();
      std::cout << value.toString().toStdString() << std::endl;

      /* in case of string value get value and convert into string*/
//      qWarning() << tr("QJsonObject[appName] of description: ") << item["description"];
//      QJsonValue subobj = item["description"];
//      qWarning() << subobj.toString();

//      /* in case of array get array and convert into string*/
//      qWarning() << tr("QJsonObject[appName] of value: ") << item["imp"];
//      QJsonArray test = item["imp"].toArray();
//      qWarning() << test[1].toString();
}
