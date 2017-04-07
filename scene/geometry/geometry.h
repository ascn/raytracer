#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

class Geometry {
public:
	Geometry() {}
	virtual ~Geometry() {}

	virtual bool intersect(const Ray &ray, Intersection *intersection) const;
}

#endif // __GEOMETRY_H__