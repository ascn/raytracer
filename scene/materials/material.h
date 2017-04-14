#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <QString>
#include <QImage>

#include <tinyobj/tiny_obj_loader.h>

enum class MaterialType { LAMBERT, PHONG, TRANSMISSIVE };


class Material {
public:
	Material();
	virtual ~Material() {}

	QString name;
	MaterialType type;
	glm::vec3 baseColor;
	bool emissive;
	float reflectivity;
	float refractive;
	QImage *texture;
	QImage *normalMap;
	float ks;
	int n;

    void convertMaterial(tinyobj::material_t material);
};

#endif // __MATERIAL_H__
