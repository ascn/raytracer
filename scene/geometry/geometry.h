#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <QString>
#include <raytracer/ray.h>
#include <raytracer/intersection.h>
#include <scene/transform.h>
#include <scene/materials/material.h>

class Intersection;

class Geometry {
public:
	Geometry() {}
	virtual ~Geometry() {}

    virtual bool intersect(const Ray &ray, Intersection *intersection) const = 0;

	QString name;
	Transform transform;
	Material *material;
};

#endif // __GEOMETRY_H__
