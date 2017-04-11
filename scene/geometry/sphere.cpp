#include <stdio.h>
#include <stdlib.h>
#include <tinyobj/tiny_obj_loader.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <math.h>
#include <scene/geometry/geometry.h>
#include <scene/geometry/sphere.h>

Sphere::Sphere(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

float Sphere::getT(float a, float b, float c) {
    float result = ((-1*b) -sqrt((b*b)-(4*a*c)))/(2*a);
    if (result < 0) {
        result = ((-1*b) +sqrt((b*b)-(4*a*c)))/(2*a);
    }
    return result;
}

bool Sphere::intersect(const Ray &ray, Intersection *intersection) const {
    float radius = 0.5;
    Ray rayMod = ray.getTransformedCopy(transform.invTransMat());
    glm::vec3 rayOriMod = rayMod.origin;
    glm::vec3 rayDirMod = rayMod.origin;
    float a = (rayDirMod[0]*rayDirMod[0]) + (rayDirMod[1]*rayDirMod[1]) + (rayDirMod[2]*rayDirMod[2]);
    float b = 2*((rayDirMod[0]*rayOriMod[0]) + (rayDirMod[1]*rayOriMod[1]) + (rayDirMod[2]*rayOriMod[2]));
    float c = (rayOriMod[0]*rayOriMod[0]) + (rayOriMod[1]*rayOriMod[1]) + (rayOriMod[2]*rayOriMod[2]) - (radius*radius);
    float discriminant = (b*b)-(4*a*c);
    if (discriminant < 0) {
        return false;
    } else {
        intersection->t = getT(a, b, c);
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::vec3((isectInit[0]/radius), (isectInit[1]/radius), (isectInit[2]/radius));
        intersection->normal = transform.transform * glm::vec4(normalInit, 0);
        return true;
    }
}
