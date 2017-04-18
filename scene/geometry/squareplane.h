#ifndef __SQUAREPLANE_H__
#define __SQUAREPLANE_H__

#include <scene/geometry/geometry.h>

class SquarePlane : public Geometry {
public:
    SquarePlane(QString name, Transform transform, Material *material);
    float getT(glm::vec3 n, glm::vec3 s, glm::vec3 origin, glm::vec3 direction) const;
    bool intersect(const Ray &ray, Intersection *intersection) const;
    float getAlpha(float y, float py, float qy);
    QRgb slerp(float alpha, QRgb az, QRgb bz);
    float slerpZ(float alpha, float az, float bz);
    glm::vec3 texMap(Intersection *intersection);


    glm::vec3 s;
};

#endif // __SQUAREPLANE_H__
