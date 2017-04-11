#ifndef __SQUAREPLANE_H__
#define __SQUAREPLANE_H__

#include <scene/geometry/geometry.h>

class SquarePlane : public Geometry {
public:
    SquarePlane(QString name, Transform transform, Material *material);
    bool intersect(const Ray &ray, Intersection *intersection) const;
};

#endif // __SQUAREPLANE_H__
