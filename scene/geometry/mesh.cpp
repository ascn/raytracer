#include <scene/geometry/mesh.h>
#include <acceleration/KdNode.h>
#include <tinyobj/tiny_obj_loader.h>
#include <vector>

Mesh::Mesh(QString name, QString objFile, Transform transform, Material *material, bool kd) {
	this->name = name;
	this->transform = transform;
	this->material = material;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	tinyobj::LoadObj(shapes, materials, objFile.toStdString().c_str());

	QString tName = name.append("\'s Triangle");
	for (const auto &s : shapes) {
		tinyobj::mesh_t mesh = s.mesh;
		for (size_t i = 0; i < mesh.indices.size(); i += 3) {
			glm::vec3 vertices[3];
			glm::vec3 normals[3];
			vertices[0] = glm::vec3(mesh.positions[mesh.indices[i] * 3],
									mesh.positions[mesh.indices[i] * 3 + 1],
									mesh.positions[mesh.indices[i] * 3 + 2]);
			vertices[1] = glm::vec3(mesh.positions[mesh.indices[i + 1] * 3],
									mesh.positions[mesh.indices[i + 1] * 3 + 1],
									mesh.positions[mesh.indices[i + 1] * 3 + 2]);
			vertices[2] = glm::vec3(mesh.positions[mesh.indices[i + 2] * 3],
									mesh.positions[mesh.indices[i + 2] * 3 + 1],
									mesh.positions[mesh.indices[i + 2] * 3 + 2]);
            Triangle *t = nullptr;
            if (mesh.normals.size() == 0) {
                glm::vec3 norm = -glm::normalize(
                		glm::cross(vertices[0] - vertices[1], vertices[2] - vertices[1]));
    			vertices[0] = transform.transform * glm::vec4(vertices[0], 1);
				vertices[1] = transform.transform * glm::vec4(vertices[1], 1);
				vertices[2] = transform.transform * glm::vec4(vertices[2], 1);
				norm = transform.invTransTrans * glm::vec4(norm, 0);
                t = new Triangle(tName, vertices, norm, material);
            } else {
				normals[0] = glm::vec3(mesh.normals[mesh.indices[i] * 3],
									   mesh.normals[mesh.indices[i] * 3 + 1],
									   mesh.normals[mesh.indices[i] * 3 + 2]);
				normals[1] = glm::vec3(mesh.normals[mesh.indices[i + 1] * 3],
									   mesh.normals[mesh.indices[i + 1] * 3 + 1],
									   mesh.normals[mesh.indices[i + 1] * 3 + 2]);
				normals[2] = glm::vec3(mesh.normals[mesh.indices[i + 2] * 3],
									   mesh.normals[mesh.indices[i + 2] * 3 + 1],
									   mesh.normals[mesh.indices[i + 2] * 3 + 2]);
				vertices[0] = transform.transform * glm::vec4(vertices[0], 1);
				vertices[1] = transform.transform * glm::vec4(vertices[1], 1);
				vertices[2] = transform.transform * glm::vec4(vertices[2], 1);
				normals[0] = transform.invTransTrans * glm::vec4(normals[0], 0);
				normals[1] = transform.invTransTrans * glm::vec4(normals[1], 0);
				normals[2] = transform.invTransTrans * glm::vec4(normals[2], 0);
                t = new Triangle(tName, vertices, normals, material);
            }
            glm::vec2 uvs[3];
            if (mesh.texcoords.size() > 0) {
                uvs[0] = glm::vec2(mesh.texcoords[mesh.indices[i] * 2],
                                   mesh.texcoords[mesh.indices[i] * 2 + 1]);
                uvs[1] = glm::vec2(mesh.texcoords[mesh.indices[i + 1] * 2],
                                   mesh.texcoords[mesh.indices[i + 1] * 2 + 1]);
                uvs[2] = glm::vec2(mesh.texcoords[mesh.indices[i + 2] * 2],
                                   mesh.texcoords[mesh.indices[i + 2] * 2 + 1]);
                t->setUVs(uvs);
            }
            this->triangles.push_back(t);
		}
	}

	if (kd) {
		kdtree = KDNode::build(triangles);
	} else {
		kdtree = nullptr;
	}
}

Mesh::~Mesh() {
	for (auto &p : triangles) { delete(p); }
}

bool Mesh::intersect(const Ray &ray, Intersection *intersection) const {
	if (kdtree != nullptr) {
		return kdtree->intersect(ray, intersection);
	}
	Intersection curr, tmp;
	bool hit = false;
	for (const auto &p : this->triangles) {
		if (p->intersect(ray, &tmp)) {
			if (curr.t < 0 || tmp.t < curr.t) {
				curr = tmp;
				hit = true;
			}
		}
	}
	if (hit) {
		intersection->isectPoint = curr.isectPoint;
		intersection->normal = curr.normal;
		intersection->objectHit = curr.objectHit;
		intersection->t = curr.t;
	}
	return hit;
}

void Mesh::mapNormal(Intersection &isect) const {

}

glm::vec3 Mesh::getColor(Intersection &isect) const {
    if (isect.objectHit->material->texture == nullptr) {
        return isect.objectHit->material->baseColor;
    }
}

