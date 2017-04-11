#include <scene/geometry/sphere.h>


Sphere::Sphere(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}

bool Sphere::intersect(const Ray &ray, Intersection *intersection) const {
    return false;
}
