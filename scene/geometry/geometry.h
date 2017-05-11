#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <QString>
#include <raytracer/ray.h>
#include <raytracer/intersection.h>
#include <scene/transform.h>
#include <scene/materials/material.h>
#include <glm/vec3.hpp>
#include <acceleration/BoundingBox.h>

class Intersection;

class Geometry {
public:
    Geometry() {}
    virtual ~Geometry() {}

    virtual bool intersect(const Ray &ray, Intersection *intersection) const = 0;
    virtual void mapNormal(Intersection &isect) const = 0;
    virtual glm::vec3 getColor(Intersection &isect) const = 0;

    QString name;
    Transform transform;
    Material *material;
    BoundingBox bbox; // Axis aligned (world space) bounding box
    int tris = 0;
protected:
	virtual BoundingBox calculateAABB() const = 0;
};

#endif // __GEOMETRY_H__
