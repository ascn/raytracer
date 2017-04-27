#ifndef __CUBE_H__
#define __CUBE_H__

#include <scene/geometry/geometry.h>
#include <QColor>

class Cube : public Geometry {
public:
    Cube(QString name, Transform transform, Material *material);
    ~Cube();
    bool intersect(const Ray &ray, Intersection *intersection) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
    glm::vec3 cubeInterp(Intersection &isect, QImage *attrib) const;
    float getAlpha(float y, float py, float qy) const;
    QRgb slerp(float alpha, QRgb az, QRgb bz) const;
};

#endif // __CUBE_H__
