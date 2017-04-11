#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <QString>
#include <QImage>

enum class MaterialType { LAMBERT, PHONG };

class Material {
public:
	Material();
	virtual ~Material() {}

	QString name;
	MaterialType type;
	glm::vec3 baseColor;
	bool emissive;
	float reflectivity;
	float refractiveIn;
	float refractiveOut;
	QImage *texture;
	QImage *normalMap;
};

#endif // __MATERIAL_H__