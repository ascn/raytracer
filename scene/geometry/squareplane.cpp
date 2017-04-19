#include <stdio.h>
#include <stdlib.h>
#include <tinyobj/tiny_obj_loader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <math.h>
#include <scene/geometry/geometry.h>
#include <scene/geometry/squareplane.h>
#include <scene/scene.h>


SquarePlane::SquarePlane(QString name, Transform transform, Material *material) {
    this->s = glm::vec3(0.5, 0.5, 0);
    this->name = name;
    this->transform = transform;
    this->material = material;
}

SquarePlane::~SquarePlane() {}

float SquarePlane::getT(glm::vec3 n, glm::vec3 s, glm::vec3 origin, glm::vec3 direction) const {
    float resultTop = glm::dot(n, s - origin);
    float resultBot = glm::dot(n, direction);
    float result = resultTop/resultBot;
    return result;
}

bool SquarePlane::intersect(const Ray &ray, Intersection *intersection) const {
    Ray rayMod = ray.getTransformedCopy(transform.invTransform);
    glm::vec3 rayOriMod = rayMod.origin;
    glm::vec3 rayDirMod = rayMod.direction;
    glm::vec3 n = glm::vec3(0, 0, 1);
    intersection->t = getT(n, glm::vec3(0.5, 0.5, 0), rayOriMod, rayDirMod);
    glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
    if (isectInit.x >= -0.5 && isectInit.x <= 0.5 &&
            isectInit.y >= -0.5 && isectInit.y <= 0.5) {
        intersection->objectHit = this;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::vec3(0, 0, 1);
        intersection->normal = transform.invTransTrans * glm::vec4(normalInit, 0);
        return true;
    }
    return false;
}

void SquarePlane::mapNormal(Intersection &isect) const {

}


glm::vec3 SquarePlane::getColor(Intersection &isect) const {
    return isect.objectHit->material->baseColor;
}

float SquarePlane::getAlpha(float y, float py, float qy) {
    float result = ((y-py)/(qy-py));
    return result;
}

QRgb SquarePlane::slerp(float alpha, QRgb az, QRgb bz) {
    float resultR = (1-alpha)*qRed(az) + alpha*qRed(bz);
    float resultG = (1-alpha)*qGreen(az) + alpha*qGreen(bz);
    float resultB = (1-alpha)*qBlue(az) + alpha*qBlue(bz);
    return qRgb(resultR, resultG, resultB);
}

glm::vec3 SquarePlane::texMap(Intersection *isect) {
    glm::vec3 isecA = transform.invTransform * glm::vec4(isect->isectPoint, 1);
    float u = isecA[0]+0.5;
    float v = isecA[1]+0.5;
    QImage *tex = this->material->texture;
    float w = tex->width();
    float h = tex->height();
    float convU = w*u;
    float convV = h*v;
    QRgb first = slerp(getAlpha(convU, ceil(convU), floor(convU)), tex->pixel(QPoint(ceil(convU), ceil(convV))), tex->pixel(QPoint(floor(convU), ceil(convV))));
    QRgb second = slerp(getAlpha(convU, ceil(convU), floor(convU)), tex->pixel(QPoint(ceil(convU), floor(convV))), tex->pixel(QPoint(floor(convU), floor(convV))));
    QRgb final = slerp(getAlpha(convV, ceil(convV), floor(convV)), first, second);
    return glm::vec3(qRed(final), qGreen(final), qBlue(final));
}
