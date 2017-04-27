#include <acceleration/BoundingBox.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <scene/geometry/triangle.h>
#include <algorithm>

bool BoundingBox::intersect(const Ray &ray, float &t) const {
	float tNear = -1 * INFINITY;
	float tFar = INFINITY;
	float t0, t1;

	// x-slab
	if (ray.direction.x == 0 && (ray.origin.x < minPoint.x || ray.origin.x > maxPoint.x)) {
		return false;
	} else {
		t0 = (minPoint.x - ray.origin.x) / ray.direction.x;
		t1 = (maxPoint.x - ray.origin.x) / ray.direction.x;
		if (t0 > t1) {
			float tmp = t0;
			t0 = t1;
			t1 = tmp;
		}
		if (t0 > tNear) { tNear = t0; }
		if (t1 < tFar) { tFar = t1; }
	}

	// y-slab
	if (ray.direction.y == 0 && (ray.origin.y < minPoint.y || ray.origin.y > maxPoint.y)) {
		return false;
	} else {
		t0 = (minPoint.y - ray.origin.y) / ray.direction.y;
		t1 = (maxPoint.y - ray.origin.y) / ray.direction.y;
		if (t0 > t1) {
			float tmp = t0;
			t0 = t1;
			t1 = tmp;
		}
		if (t0 > tNear) { tNear = t0; }
		if (t1 < tFar) { tFar = t1; }
	}

	// z-slab
	if (ray.direction.z == 0 && (ray.origin.z < minPoint.z || ray.origin.z > maxPoint.z)) {
		return false;
	} else {
		t0 = (minPoint.z - ray.origin.z) / ray.direction.z;
		t1 = (maxPoint.z - ray.origin.z) / ray.direction.z;
		if (t0 > t1) {
			float tmp = t0;
			t0 = t1;
			t1 = tmp;
		}
		if (t0 > tNear) { tNear = t0; }
		if (t1 < tFar) { tFar = t1; }
	}

	if (tNear > tFar) { return false; }
	t = tNear;
	return true;
}

BoundingBox BoundingBox::Union(const BoundingBox &bb1, const BoundingBox &bb2) {
	BoundingBox ret;
	ret.minPoint.x = std::min(bb1.minPoint.x, bb2.minPoint.x);
	ret.minPoint.y = std::min(bb1.minPoint.y, bb2.minPoint.y);
	ret.minPoint.z = std::min(bb1.minPoint.z, bb2.minPoint.z);
	ret.maxPoint.x = std::max(bb1.maxPoint.x, bb2.maxPoint.x);
	ret.maxPoint.y = std::max(bb1.maxPoint.y, bb2.maxPoint.y);
	ret.maxPoint.z = std::max(bb1.maxPoint.z, bb2.maxPoint.z);
	return ret;
}

BoundingBox getAABB(std::vector<Triangle *> &primitives) {
	BoundingBox ret;
	if (primitives.size() == 0) {
		return ret;
	}
	ret = primitives[0]->bbox;
    for (unsigned int i = 1; i < primitives.size(); ++i) {
		ret = BoundingBox::Union(ret, primitives[i]->bbox);
	}
	return ret;
}

Axis BoundingBox::getLongestAxis() const {
	float xLength = maxPoint.x - minPoint.x;
	float yLength = maxPoint.y - minPoint.y;
	float zLength = maxPoint.z - minPoint.z;
	if (xLength > yLength && xLength > zLength) {
		return Axis::X;
	} else if (yLength > xLength && yLength > zLength) {
		return Axis::Y;
	} else {
		return Axis::Z;
	}
}

glm::vec3 BoundingBox::getMidpoint() const {
	return glm::vec3((minPoint.x + maxPoint.x) / 2,
                     (minPoint.y + maxPoint.y) / 2,
                     (minPoint.z + maxPoint.z) / 2);
}
