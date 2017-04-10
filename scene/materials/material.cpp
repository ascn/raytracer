#include <scene/materials/material.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <QString>
#include <QImage>

Material::Material() : 
	name("Material"), baseColor(glm::vec3(0.1f, 0.1f, 0.1f)),
	emissive(false), reflectivity(0), refractiveIn(1), refractiveOut(1),
	texture(nullptr), normalMap(nullptr)
{}