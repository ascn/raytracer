
#include <scene/geometry/cube.h>


Cube::Cube(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

bool Cube::intersect(const Ray &ray, Intersection *intersection) const {
    return false;
}
