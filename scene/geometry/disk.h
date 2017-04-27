#ifndef DISK_H
#define DISK_H

#include <scene/geometry/geometry.h>
#include <glm/vec3.hpp>

class Disk : public Geometry {
public:
    Disk(QString name, Transform transform, Material *material);
    ~Disk();
    float getT(glm::vec3 n, glm::vec3 s, glm::vec3 origin, glm::vec3 direction) const;
    bool intersect(const Ray &ray, Intersection *intersection) const;
    glm::vec3 spInterpolation(Intersection &isect, QImage *attrib) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
    float getAlpha(float y, float py, float qy) const;
    QRgb slerp(float alpha, QRgb az, QRgb bz) const;
    float slerpZ(float alpha, float az, float bz);

    glm::vec3 s;
};

#endif // DISK_H
