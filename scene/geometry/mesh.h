#ifndef __MESH_H__
#define __MESH_H__

#include <scene/geometry/geometry.h>
#include <scene/geometry/triangle.h>
#include <acceleration/KdNode.h>
#include <vector>

class Mesh : public Geometry {
public:
    Mesh(QString name, QString objFile, Transform transform, Material *material, bool kd);
	~Mesh();
	bool intersect(const Ray &ray, Intersection *intersection) const;
    void mapNormal(Intersection &isect) const;
    glm::vec3 getColor(Intersection &isect) const;
    BoundingBox calculateAABB() const;
    KDNode *kdtree;
	std::vector<Geometry *> triangles; // all stored in world space
};

#endif // __MESH_H__
