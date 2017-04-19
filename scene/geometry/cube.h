#ifndef __CUBE_H__
#define __CUBE_H__

#include <scene/geometry/geometry.h>

class Cube : public Geometry {
public:
    Cube(QString name, Transform transform, Material *material);
    ~Cube();
    bool intersect(const Ray &ray, Intersection *intersection) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
};

#endif // __CUBE_H__
