#include <scene/geometry/cube.h>


Cube::Cube(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

Cube::~Cube() {}

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
        intersection->objectHit = this;
        glm::vec3 isectInit = rayOriMod + intersection->t*rayDirMod;
        intersection->isectPoint = transform.transform * glm::vec4(isectInit, 1);
        glm::vec3 absIsect = glm::abs(isectInit);
        float max = glm::max(absIsect.x, glm::max(absIsect.y, absIsect.z));
        glm::vec3 normalInit;
        if (max == absIsect.x) {
            if (isectInit.x < 0) {
                normalInit = glm::vec3(-1, 0, 0);
            } else {
                normalInit = glm::vec3(1, 0, 0);
            }
        } else if (max == absIsect.y) {
            if (isectInit.y < 0) {
                normalInit = glm::vec3(0, -1, 0);
            } else {
                normalInit = glm::vec3(0, 1, 0);
            }
        } else {
            if (isectInit.z < 0) {
                normalInit = glm::vec3(0, 0, -1);
            } else {
                normalInit = glm::vec3(0, 0, 1);
            }
        }
        intersection->normal = transform.invTransTrans * glm::vec4(normalInit, 0);
        return true;
    }
}

void Cube::mapNormal(Intersection &isect) const {

}

glm::vec3 Cube::getColor(Intersection &isect) const {
    if (isect.objectHit->material->texture->isNull()) {
        return isect.objectHit->material->baseColor;
    }
}
