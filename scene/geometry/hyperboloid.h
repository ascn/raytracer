#ifndef HYPERBOLOID_H
#define HYPERBOLOID_H

#include <scene/geometry/geometry.h>
#include <glm/vec3.hpp>
#include <QColor>

class Hyperboloid : public Geometry {
public:
    Hyperboloid(QString name, Transform transform, Material *material);
    ~Hyperboloid();
    bool intersect(const Ray &ray, Intersection *intersection) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
    float getAlpha(float y, float py, float qy) const;
    QRgb slerp(float alpha, QRgb az, QRgb bz) const;
    glm::vec3 sphereInterpolation(Intersection &isect, QImage *attrib) const;
    BoundingBox calculateAABB() const;
};

#endif // HYPERBOLOID_H
