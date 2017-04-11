#include <stdio.h>
#include <stdlib.h>
#include <tinyobj/tiny_obj_loader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <math.h>
#include <scene/geometry/geometry.h>
#include <scene/geometry/squareplane.h>

SquarePlane::SquarePlane(QString name, Transform transform, Material material) {
    this->s = glm::vec3(0.5, 0.5, 0);
    this->name = name;
    this->transform = transform;
    this->material = material;
}

float SquarePlane::dotproduct(glm::vec3 a, glm::vec3 b) {
    float result = (a[0]*b[0])+(a[1]*b[1])+(a[2]*b[2]);
    return result;
}

float SquarePlane::getT(glm::vec3 n, glm::vec3 s, glm::vec3 origin, glm::vec3 direction) {
    float resultTop = dotproduct(n, s.operator -=(origin));
    float resultBot = dotproduct(n, direction);
    float result = resultTop/resultBot;
    return result;
}

bool SquarePlane::intersect(const Ray &ray, Intersection *intersection) const {
    Ray rayMod = ray.getTransformedCopy(transform.invTransMat());
    glm::vec3 rayOriMod = rayMod.origin;
    glm::vec3 rayDirMod = rayMod.direction;
    glm::vec3 n = glm::vec3(0, 0, 0.5);
    intersection->t = getT(n, glm::vec3(0.5, 0.5, 0), rayOriMod, rayDirMod);
    glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
    intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
    glm::vec3 normalInit = glm::vec3(isectInit[0], isectInit[1], -0.5);
    intersection->normal = transform.transform * glm::vec4(normalInit, 0);
    return true;
}
