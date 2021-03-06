#include <stdio.h>
#include <stdlib.h>
#include <tinyobj/tiny_obj_loader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <math.h>
#include <scene/geometry/geometry.h>
#include <scene/geometry/paraboloid.h>
#include <QColor>

Paraboloid::Paraboloid(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

Paraboloid::~Paraboloid() {}

bool Paraboloid::intersect(const Ray &ray, Intersection *intersection) const {
    Ray rayMod = ray.getTransformedCopy(transform.invTransform);
    glm::vec3 rayOriMod = rayMod.origin;
    glm::vec3 rayDirMod = rayMod.direction;
    float a = (rayDirMod[0]*rayDirMod[0]) + (rayDirMod[2]*rayDirMod[2]);
    float b = 2*((rayDirMod[0]*rayOriMod[0]) + (rayDirMod[2]*rayOriMod[2])) - rayDirMod[1];
    float c = (rayOriMod[0]*rayOriMod[0]) + (rayOriMod[2]*rayOriMod[2]) - rayOriMod[1];

    float t0 = ((-1*b) -sqrt((b*b)-(4*a*c)))/(2*a);
    float t1 = ((-1*b) +sqrt((b*b)-(4*a*c)))/(2*a);
    if (t1 < t0) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
    float y0 = rayOriMod[1]+(t0*rayDirMod[1]);
    float y1 = rayOriMod[1]+(t1*rayDirMod[1]);

    if ((y0 > 0.5) && (y1 > 0.5)) {
        return false;
    } else if ((y0 > 0.5) && (y1 < 0.5)) {
        intersection->t = t0 + (t1-t0) * (y0-0.5) / (y0-y1);
        intersection->objectHit = this;
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::vec3(0, 1, 0);
        intersection->normal = transform.invTransTrans * glm::vec4(normalInit, 0);
        return true;
    } else if ((y0 < 0.5) && (y0 > -0.5)) {
        intersection->t = t0;
        intersection->objectHit = this;
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::normalize(glm::vec3(isectInit.x, isectInit.y, isectInit.z));
        intersection->normal = transform.invTransTrans * glm::vec4(normalInit, 0);
        return true;
    } else if ((y0 < -0.5) && (y1 > -0.5)) {
        intersection->t = t0;
        intersection->objectHit = this;
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::vec3(0, -1, 0);
        intersection->normal = transform.invTransTrans * glm::vec4(normalInit, 0);
        return true;
    } else {
        return false;
    }

}

void Paraboloid::mapNormal(Intersection &isect) const {
    if (!(isect.objectHit->material->normalMap == nullptr)) {
        glm::vec3 normalInit = transform.invTransform * glm::vec4(isect.normal, 0);
        glm::vec3 tangent = glm::normalize(glm::cross(glm::vec3(0, 1, 0), normalInit));
        glm::vec3 bitangent = glm::normalize(glm::cross(normalInit, tangent));
        glm::mat4 trans = glm::mat4(glm::vec4(tangent, 0), glm::vec4(bitangent, 0),
                                    glm::vec4(normalInit, 0), glm::vec4(0, 0, 0, 1));
        glm::vec3 mapNorm = sphereInterpolation(isect, material->normalMap);
        mapNorm = (mapNorm * glm::vec3(255) - glm::vec3(128)) / glm::vec3(128);
        isect.normal = trans * glm::vec4(mapNorm, 0);
    }
}

glm::vec3 Paraboloid::sphereInterpolation(Intersection &isect, QImage *attrib) const {
    glm::vec3 normA = transform.invTransform * glm::vec4(isect.normal, 0);
    float a = atan2f(normA[2], normA[0]);
    const float PI = 3.1415927;
    if (a < 0) {
        const float PI = 3.1415927;
        a = a+2*PI;
    }
    float b = acos(normA[1]);
    float u = 1 - (a/(2*PI));
    float v = 1 - (b/PI);
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


glm::vec3 Paraboloid::getColor(Intersection &isect) const {

    if (isect.objectHit->material->texture == nullptr) {
        return isect.objectHit->material->baseColor;
    } else {
        QImage *tex = this->material->texture;
        return sphereInterpolation(isect, tex);
    }
}

float Paraboloid::getAlpha(float y, float py, float qy) const {
    float result = ((y-py)/(qy-py));
    return result;
}

QRgb Paraboloid::slerp(float alpha, QRgb az, QRgb bz) const {
    float resultR = (1-alpha)*qRed(az) + alpha*qRed(bz);
    float resultG = (1-alpha)*qGreen(az) + alpha*qGreen(bz);
    float resultB = (1-alpha)*qBlue(az) + alpha*qBlue(bz);
    return qRgb(resultR, resultG, resultB);
}

BoundingBox Paraboloid::calculateAABB() const {
    BoundingBox ret;
    return ret;
}
