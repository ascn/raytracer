#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "geometry.h"

class Triangle : public Geometry {
    Triangle(QString name, Transform transform, Material *material);
    bool intersect(const Ray &ray, Intersection *intersection) const;
};

#endif // __TRIANGLE_H__
