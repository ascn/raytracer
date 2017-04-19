#include <stdio.h>
#include <stdlib.h>
#include <tinyobj/tiny_obj_loader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <math.h>
#include <scene/geometry/geometry.h>
#include <scene/geometry/sphere.h>
#include <QColor>

Sphere::Sphere(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

Sphere::~Sphere() {}

float Sphere::getT(float a, float b, float c) const {
    float result = ((-1*b) -sqrt((b*b)-(4*a*c)))/(2*a);
    if (result < 0) {
        result = ((-1*b) +sqrt((b*b)-(4*a*c)))/(2*a);
    }
    return result;
}

bool Sphere::intersect(const Ray &ray, Intersection *intersection) const {
    float radius = 0.5;
    Ray rayMod = ray.getTransformedCopy(transform.invTransform);
    glm::vec3 rayOriMod = rayMod.origin;
    glm::vec3 rayDirMod = rayMod.direction;
    float a = (rayDirMod[0]*rayDirMod[0]) + (rayDirMod[1]*rayDirMod[1]) + (rayDirMod[2]*rayDirMod[2]);
    float b = 2*((rayDirMod[0]*rayOriMod[0]) + (rayDirMod[1]*rayOriMod[1]) + (rayDirMod[2]*rayOriMod[2]));
    float c = (rayOriMod[0]*rayOriMod[0]) + (rayOriMod[1]*rayOriMod[1]) + (rayOriMod[2]*rayOriMod[2]) - (radius*radius);
    float discriminant = (b*b)-(4*a*c);
    if (discriminant < 0) {
        return false;
    } else {
        intersection->t = getT(a, b, c);
        intersection->objectHit = this;
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::normalize(glm::vec3(isectInit.x, isectInit.y, isectInit.z));
        intersection->normal = transform.invTransTrans * glm::vec4(normalInit, 0);
        return true;
    }
}

void Sphere::mapNormal(Intersection &isect) const {

    if (!isect.objectHit->material->normalMap->isNull()) {
        QImage *nm = this->material->normalMap;
        isect.normal = sphereInterpolation(isect, nm);
    }
}

glm::vec3 Sphere::sphereInterpolation(Intersection &isect, QImage *attrib) const {
    glm::vec3 normA = transform.invTransform * glm::vec4(isect.normal, 0);
    float a = atan2f(normA[2], normA[0]);
    const float PI = 3.1415927;
    if (a < 0) {
        const float PI = 3.1415927;
        a = a+2*PI;
    }
    float b = acos(normA[1]);
    float u = 1-(a/(2*PI));
    float v = 1-(b/PI);

    float w = attrib->width();
    float h = attrib->height();
    float convU = w*u;
    float convV = h*v;
    QRgb first = slerp(getAlpha(convU, ceil(convU), floor(convU)), attrib->pixel(QPoint(ceil(convU), ceil(convV))), attrib->pixel(QPoint(floor(convU), ceil(convV))));
    QRgb second = slerp(getAlpha(convU, ceil(convU), floor(convU)), attrib->pixel(QPoint(ceil(convU), floor(convV))), attrib->pixel(QPoint(floor(convU), floor(convV))));
    QRgb final = slerp(getAlpha(convV, ceil(convV), floor(convV)), first, second);
    return glm::vec3(qRed(final), qGreen(final), qBlue(final));
}


glm::vec3 Sphere::getColor(Intersection &isect) const {

    if (isect.objectHit->material->texture->isNull()) {
        return isect.objectHit->material->baseColor;
    } else {
        QImage *tex = this->material->normalMap;
        return sphereInterpolation(isect, tex);
    }
}

float Sphere::getAlpha(float y, float py, float qy) const {
    float result = ((y-py)/(qy-py));
    return result;
}

QRgb Sphere::slerp(float alpha, QRgb az, QRgb bz) const {
    float resultR = (1-alpha)*qRed(az) + alpha*qRed(bz);
    float resultG = (1-alpha)*qGreen(az) + alpha*qGreen(bz);
    float resultB = (1-alpha)*qBlue(az) + alpha*qBlue(bz);
    return qRgb(resultR, resultG, resultB);
}
