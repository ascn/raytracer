#ifndef __KDNODE_H__
#define __KDNODE_H__

#include <vector>
#include <acceleration/BoundingBox.h>
#include <raytracer/intersection.h>

class KDNode {
public:
	KDNode();
	bool intersect(const Ray &ray, Intersection *intersection) const;
	static KDNode *build(std::vector<Triangle *> &triangles);

	BoundingBox bbox;
	KDNode *left;
	KDNode *right;
	std::vector<Triangle *> triangles;
};

#endif // __KDNODE_H__