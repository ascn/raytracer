#ifndef __CUBE_H__
#define __CUBE_H__

#include "geometry.h"

class Cube : public Geometry
{
    Cube(QString name, Transform transform, Material *material);
};

#endif
