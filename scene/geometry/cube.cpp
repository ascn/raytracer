#include <scene/geometry/cube.h>
#include <QDebug>

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
    if (isect.objectHit->material->texture == nullptr) {
        return isect.objectHit->material->baseColor;
    }
    return cubeInterp(isect, material->texture);
}

glm::vec3 Cube::cubeInterp(Intersection &isect, QImage *attrib) const {
    glm::vec3 isectA = transform.invTransform * glm::vec4(isect.isectPoint, 1);
    float u, v;
    glm::vec3 absIsect = glm::abs(isectA);
    float max = glm::max(absIsect.x, glm::max(absIsect.y, absIsect.z));
    if (max == absIsect.x) {
        if (isectA.x < 0) {
            u = (isectA.y + 0.5) / 4 + 0.25;
            v = (isectA.z + 0.5) / 3;
        } else {
            u = (isectA.y + 0.5) / 4 + 0.25;
            v = (isectA.z + 0.5) / 3 + 0.66;
        }
    } else if (max == absIsect.y) {
        if (isectA.y < 0) {
            u = (isectA.x + 0.5) / 4 + 0.25;
            v = (isectA.z + 0.5) / 3 + 0.33;
        } else {
            u = (isectA.x + 0.5) / 4 + 0.75;
            v = (isectA.z + 0.5) / 3 + 0.33;
        }
    } else {
        if (isectA.z < 0) {
            u = (isectA.x + 0.5) / 4;
            v = (isectA.y + 0.5) / 3 + 0.33;
        } else {
            u = (isectA.x + 0.5) / 4 + 0.5;
            v = (isectA.y + 0.5) / 3 + 0.33;
        }
    }
    float w = attrib->width() - 1;
    float h = attrib->height() - 1;
    float convU = w * u;
    float convV = h * v;
    QRgb first = slerp(getAlpha(convU, ceil(convU), floor(convU)), attrib->pixel(QPoint(ceil(convU), ceil(convV))), attrib->pixel(QPoint(floor(convU), ceil(convV))));
    QRgb second = slerp(getAlpha(convU, ceil(convU), floor(convU)), attrib->pixel(QPoint(ceil(convU), floor(convV))), attrib->pixel(QPoint(floor(convU), floor(convV))));
    QRgb final = slerp(getAlpha(convV, ceil(convV), floor(convV)), first, second);
    glm::vec3 ret = glm::vec3(qRed(final), qGreen(final), qBlue(final));
    return ret / glm::vec3(255);
}

float Cube::getAlpha(float y, float py, float qy) const {
    float result = ((y-py)/(qy-py));
    return result;
}

QRgb Cube::slerp(float alpha, QRgb az, QRgb bz) const {
    float resultR = (1-alpha)*qRed(az) + alpha*qRed(bz);
    float resultG = (1-alpha)*qGreen(az) + alpha*qGreen(bz);
    float resultB = (1-alpha)*qBlue(az) + alpha*qBlue(bz);
    return qRgb(resultR, resultG, resultB);
}
