
#include <scene/geometry/cube.h>


Cube::Cube(QString name, Transform transform, Material *material) {
    this->name = name;
    this->transform = transform;
    this->material = material;
}
