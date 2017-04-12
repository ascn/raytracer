#include <stdio.h>
#include <stdlib.h>
#include <tinyobj/tiny_obj_loader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <math.h>
#include <scene/geometry/geometry.h>
#include <scene/geometry/squareplane.h>

SquarePlane::SquarePlane(QString name, Transform transform, Material *material) {
    this->s = glm::vec3(0.5, 0.5, 0);
    this->name = name;
    this->transform = transform;
    this->material = material;
}

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
