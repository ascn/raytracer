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
    if (!(isect.objectHit->material->normalMap == nullptr)) {
        glm::vec3 mapNorm = spInterpolation(isect, material->normalMap);
        mapNorm = (mapNorm * glm::vec3(255) - glm::vec3(128)) / glm::vec3(128);
        isect.normal = transform.invTransTrans * glm::vec4(mapNorm, 0);
    }
}

glm::vec3 SquarePlane::getColor(Intersection &isect) const {
    if (isect.objectHit->material->texture == nullptr) {
        return isect.objectHit->material->baseColor;
    }

    QImage *tex = this->material->texture;
    return spInterpolation(isect, tex);
}

glm::vec3 SquarePlane::spInterpolation(Intersection &isect, QImage *attrib) const {
    glm::vec3 isecA = transform.invTransform * glm::vec4(isect.isectPoint, 1);
    float u = isecA[0]+0.5;
    float v = isecA[1]+0.5;

    float w = attrib->width() - 1;
    float h = attrib->height() - 1;
    u = u < 0.5 ? u + 2 * (0.5 - u) : u - 2 * (u - 0.5);
    float convU = w*u;
    float convV = h*v;
    QRgb first = slerp(getAlpha(convU, ceil(convU), floor(convU)), attrib->pixel(QPoint(ceil(convU), ceil(convV))), attrib->pixel(QPoint(floor(convU), ceil(convV))));
    QRgb second = slerp(getAlpha(convU, ceil(convU), floor(convU)), attrib->pixel(QPoint(ceil(convU), floor(convV))), attrib->pixel(QPoint(floor(convU), floor(convV))));
    QRgb final = slerp(getAlpha(convV, ceil(convV), floor(convV)), first, second);
    glm::vec3 ret = glm::vec3(qRed(final), qGreen(final), qBlue(final));
    return ret / glm::vec3(255);
}

float SquarePlane::getAlpha(float y, float py, float qy) const {
    float result = ((y-py)/(qy-py));
    return result;
}

QRgb SquarePlane::slerp(float alpha, QRgb az, QRgb bz) const {
    float resultR = (1-alpha)*qRed(az) + alpha*qRed(bz);
    float resultG = (1-alpha)*qGreen(az) + alpha*qGreen(bz);
    float resultB = (1-alpha)*qBlue(az) + alpha*qBlue(bz);
    return qRgb(resultR, resultG, resultB);
}

