#ifndef __CUBE_H__
#define __CUBE_H__

#include "geometry.h"

class Cube : public Geometry
{
    Cube(QString name, Transform transform, Material *material);
    bool intersect(const Ray &ray, Intersection *intersection) const;
};

#endif
