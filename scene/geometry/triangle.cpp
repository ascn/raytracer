#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/vec3.hpp>
#include <scene/geometry/triangle.h>

Triangle::Triangle(QString name, glm::vec3 vertices[3],
		glm::vec3 normals[3], Material *material) {
	this->name = name;
	this->vertices[0] = vertices[0];
	this->vertices[1] = vertices[1];
	this->vertices[2] = vertices[2];
	this->normals[0] = normals[0];
	this->normals[1] = normals[1];
	this->normals[2] = normals[2];
	this->material = material;
	this->S = getArea(vertices[0], vertices[1], vertices[2]);
}

bool Triangle::intersect(const Ray &ray, Intersection *intersection) const {
	glm::vec3 normal = glm::normalize(
			glm::cross(vertices[0] - vertices[1], vertices[2] - vertices[1]));

	float D = glm::dot(normal, vertices[0]);
	if (D == 0) { return false; }
	float t = (glm::dot(normal, vertices[0] - ray.origin)) / glm::dot(normal, ray.direction);
	if (t < 0) { return false; }
	glm::vec3 P = ray.origin + t * ray.direction;

	float S1 = getArea(P, vertices[1], vertices[2]) / S;
	float S2 = getArea(P, vertices[2], vertices[0]) / S;
	float S3 = getArea(P, vertices[0], vertices[1]) / S;
	if (0 <= S1 && S1 <= 1 &&
		0 <= S2 && S2 <= 1 &&
		0 <= S3 && S3 <= 1 && (S1 + S2 + S3 - 1 < 0.001)) {
		intersection->t = t;
		intersection->normal = getNormal(P);
		intersection->objectHit = this;
		intersection->isectPoint = P;
		return true;
	}
	return false;
}

float Triangle::getArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const {
	glm::vec3 AB = p2 - p1;
	glm::vec3 AC = p3 - p1;
	return 0.5 * glm::length(glm::cross(AB, AC));
}

glm::vec3 Triangle::getNormal(glm::vec3 point) const {

	glm::vec3 v0 = vertices[1] - vertices[0];
	glm::vec3 v1 = vertices[2] - vertices[0];
	glm::vec3 v2 = point - vertices[0];
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float d = d00 * d11 - d01 * d01;
	float l0 = (d11 * d20 - d01 * d21) / d;
	float l1 = (d00 * d21 - d01 * d20) / d;
	float l2 = 1 - l0 - l1;

    return (normals[0] * l2 + normals[1] * l0 + normals[2] * l1);
}

glm::vec3 Triangle::triangleInterpolation(glm::vec3 P, const glm::vec3 *vertices, const glm::vec3 *attributes) const {

    float S = getArea(vertices[0], vertices[1], vertices[2]);
    float S1 = getArea(P, vertices[1], vertices[2]) / S;
    float S2 = getArea(P, vertices[2], vertices[0]) / S;
    float S3 = getArea(P, vertices[0], vertices[1]) / S;

    glm::vec3 result;
    // x
    result[0] = attributes[0][0] * (S1/S) + attributes[1][0] * (S2/S) + attributes[2][0]*(S3/S);
    // y
    result[1] = attributes[0][1] * (S1/S) + attributes[1][1] * (S2/S) + attributes[2][1]*(S3/S);
    // z
    result[2] = attributes[0][2] * (S1/S) + attributes[1][2] * (S2/S) + attributes[2][2]*(S3/S);
    return result;
}

void Triangle::mapNormal(Intersection &isect) const {

    const Triangle* obj = dynamic_cast<const Triangle*>(isect.objectHit);
    if (!isect.objectHit->material->normalMap->isNull()) {
        QRgb p1 = isect.objectHit->material->normalMap->pixel(obj->vertices[0][0], obj->vertices[0][1]);
        QRgb p2 = isect.objectHit->material->normalMap->pixel(obj->vertices[1][0], obj->vertices[1][1]);
        QRgb p3 = isect.objectHit->material->normalMap->pixel(obj->vertices[2][0], obj->vertices[2][1]);

        glm::vec3 attributes[3];
        attributes[0][0] = qRed(p1);
        attributes[0][1] = qRed(p1);
        attributes[0][2] = qRed(p1);
        attributes[1][0] = qRed(p2);
        attributes[1][1] = qRed(p2);
        attributes[1][2] = qRed(p2);
        attributes[2][0] = qRed(p3);
        attributes[2][1] = qRed(p3);
        attributes[2][2] = qRed(p3);

        isect.normal = triangleInterpolation(isect.isectPoint, obj->vertices, attributes);
    } else {
        isect.normal = triangleInterpolation(isect.isectPoint, obj->vertices, obj->normals);
    }
}

glm::vec3 Triangle::getColor(Intersection &isect) const {

    if (isect.objectHit->material->texture->isNull()) {
        return isect.objectHit->material->baseColor;
    }
    const Triangle *obj = dynamic_cast<const Triangle *>(isect.objectHit);
    QRgb p1 = isect.objectHit->material->texture->pixel(obj->vertices[0][0], obj->vertices[0][1]);
    QRgb p2 = isect.objectHit->material->texture->pixel(obj->vertices[1][0], obj->vertices[1][1]);
    QRgb p3 = isect.objectHit->material->texture->pixel(obj->vertices[2][0], obj->vertices[2][1]);

    glm::vec3 attributes[3];
    attributes[0][0] = qRed(p1);
    attributes[0][1] = qRed(p1);
    attributes[0][2] = qRed(p1);
    attributes[1][0] = qRed(p2);
    attributes[1][1] = qRed(p2);
    attributes[1][2] = qRed(p2);
    attributes[2][0] = qRed(p3);
    attributes[2][1] = qRed(p3);
    attributes[2][2] = qRed(p3);

    return triangleInterpolation(isect.isectPoint, obj->vertices, attributes);
}


