#include <scene/materials/material.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <QString>
#include <QImage>

Material::Material() : 
	name("Material"), type(MaterialType::LAMBERT),
	baseColor(glm::vec3(0.1f, 0.1f, 0.1f)),
	emissive(false), reflectivity(0), refractiveIn(0), refractiveOut(0),
	texture(nullptr), normalMap(nullptr)
{}

