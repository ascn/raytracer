#include <scene/geometry/squareplane.h>


SquarePlane::SquarePlane(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

bool SquarePlane::intersect(const Ray &ray, Intersection *intersection) const {
    return false;
}
