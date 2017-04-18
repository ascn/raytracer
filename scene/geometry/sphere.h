#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <scene/geometry/geometry.h>
#include <glm/vec3.hpp>

class Sphere : public Geometry {
public:
    Sphere(QString name, Transform transform, Material *material);
    ~Sphere();
    float getT(float a, float b, float c) const;
    bool intersect(const Ray &ray, Intersection *intersection) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
    float getAlpha(float y, float py, float qy);
    QRgb slerp(float alpha, QRgb az, QRgb bz);
    glm::vec3 texMap(Intersection *intersection);
};

#endif // __SPHERE_H__
