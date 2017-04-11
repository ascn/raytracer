#include <scene/geometry/cube.h>


Cube::Cube(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

bool Cube::intersect(const Ray &ray, Intersection *intersection) const {
    float tNear = (-1*INFINITY);
    float tFar = INFINITY;
    float t0;
    float t1;
    Ray rayMod = ray.getTransformedCopy(transform.invTransform);
    glm::vec3 rayOriMod = rayMod.origin;
    glm::vec3 rayDirMod = rayMod.direction;

    // x-slab=================================================================================================
    if ((rayDirMod[0] == 0) && ((rayOriMod[0] < -0.5) || (rayOriMod[0] > 0.5))) {
        return false;
    } else {
        t0 = ((-0.5-rayOriMod[0])/rayDirMod[0]);
        t1 = ((0.5-rayOriMod[0])/rayDirMod[0]);
        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        if (t0 > tNear) {
            tNear = t0;
        }
        if (t1 < tFar) {
            tFar = t1;
        }
    }
    // y-slab==================================================================================================
    if ((rayDirMod[1] == 0) && ((rayOriMod[1] < -0.5) || (rayOriMod[1] > 0.5))) {
        return false;
    } else {
        t0 = ((-0.5-rayOriMod[1])/rayDirMod[1]);
        t1 = ((0.5-rayOriMod[1])/rayDirMod[1]);
        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        if (t0 > tNear) {
            tNear = t0;
        }
        if (t1 < tFar) {
            tFar = t1;
        }
    }
    // z-slab==================================================================================================
    if ((rayDirMod[2] == 0) && ((rayOriMod[2] < -0.5) || (rayOriMod[2] > 0.5))) {
        return false;
    } else {
        t0 = ((-0.5-rayOriMod[2])/rayDirMod[2]);
        t1 = ((0.5-rayOriMod[2])/rayDirMod[2]);
        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        if (t0 > tNear) {
            tNear = t0;
        }
        if (t1 < tFar) {
            tFar = t1;
        }
    }

    if (tNear > tFar) {
        return false;
    } else {
        intersection->t = tNear;
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 normalInit = glm::vec3(isectInit[0], isectInit[1], -0.5);
        intersection->normal = transform.transform * glm::vec4(normalInit, 0);
        return true;
    }
}
