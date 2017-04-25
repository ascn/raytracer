#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>
#include <raytracer/RaytraceEngine.h>
#include <scene/camera.h>
#include <scene/scene.h>
#include <raytracer/ray.h>
#include <raytracer/intersection.h>
#include <scene/geometry/geometry.h>
#include <sampler/sampler.h>

#include <QImage>
#include <QColor>
#include <QDebug>
#include <cstdlib>
#include <raytracer/RenderTask.h>
#include <QThreadPool>
#include <QVector>
#include <QThread>
#include <vector>

RaytraceEngine::RaytraceEngine() {}

void RaytraceEngine::render(const Camera &camera, const Scene &scene, QImage &image,
                            uint8_t maxDepth, uint8_t samples, bool multithreading) {
	// For each pixel, cast *samples* rays using traceRay.
	// Average the color over all samples from traceRay and set
	// color of pixel (SSAA).

	if (!multithreading) {
		Sampler sampler;
		for (int i = 0; i < camera.width; ++i) {
			for (int j = 0; j < camera.height; ++j) {
				glm::vec3 total = glm::vec3(0);
                if (i == 256 && j == 252) {
                    int k = 0;
                }
                std::vector<glm::vec3> points;
                sampler.generateSamples(samples, points, Warp::Square);
                for (int k = 0; k < points.size(); ++k) {
                	glm::vec3 v = points[k];
                	Ray ray = camera.raycast(i + v.x, j + v.y);
                	glm::vec3 color = traceRay(ray, scene, 0, maxDepth, samples, k);
                	total += color;
                }
				total /= glm::vec3(samples);
                image.setPixel(i, j, qRgb(total.x, total.y, total.z));
                //QRgb *line = (QRgb *) image.scanLine(j);
                //line += i;
                //*line = qRgb(total.x, total.y, total.z);
	        }
	        emit updateGUI();
		}
	} else {
		QThreadPool *qtp = QThreadPool::globalInstance();
		QVector<RenderTask *> taskList;
		qtp->setMaxThreadCount(QThread::idealThreadCount() - 1);

		for (int i = 0; i < camera.width; i += 16) {
			for (int j = 0; j < camera.height; j += 16) {
				glm::vec2 minPoint;
				glm::vec2 maxPoint;
				minPoint.x = i;
				minPoint.y = j;
				if (i + 16 > camera.width) {
					maxPoint.x = camera.width;
				} else {
					maxPoint.x = i + 16;
				}
				if (j + 16 > camera.height) {
					maxPoint.y = camera.height;
				} else {
					maxPoint.y = j + 16;
				}
				taskList.push_back(new RenderTask(
					minPoint, maxPoint, &camera, &scene, image, maxDepth, samples, this));
				qtp->start(taskList.back());
			}
		}
		qtp->waitForDone(-1);
	}
}

glm::vec3 RaytraceEngine::traceRay(const Ray &ray, const Scene &scene, 
								   uint8_t depth, uint8_t maxDepth, uint8_t samples,
								   uint8_t idx) {
	// Recursively traces a ray up to maxDepth. If the ray hits a geometry,
	// we cast light feeler rays, add up contribution, and divide by number
	// lights. If the geometry hit is not reflective or transmissive, we
	// stop even if current recursion depth isn't max depth. Otherwise we
	// reflect/refract the ray and continue.

    Intersection isect = Intersection::getIntersection(ray, scene);
    if (isect.objectHit == nullptr) { return glm::vec3(25, 25, 25); }
    if (depth > maxDepth) { return isect.objectHit->getColor(isect); }
    const glm::vec3 color = isect.objectHit->getColor(isect);

	if (isect.objectHit->material->emissive) {
        return color * glm::vec3(255);
	}

	switch (isect.objectHit->material->type) {
    case MaterialType::LAMBERT: {
		glm::vec3 total = glm::vec3(0);
	    for (auto &p : scene.lights) {
	        Ray feeler = isect.raycast(p->getCastPoint(samples, idx));
			if (Intersection::getIntersection(feeler, scene).objectHit == p->geometry) {
                total += color * glm::max(0.f, glm::dot(glm::normalize(isect.normal),
                                 glm::normalize(feeler.direction))) *
						glm::vec3(255);
			}
		}

		glm::vec3 rColor = glm::vec3(0);
		if (isect.objectHit->material->reflectivity > 0) {
			glm::vec3 reflectDirection = glm::reflect(ray.direction, isect.normal);
			Ray reflect;
			reflect.origin = isect.isectPoint + glm::vec3(0.01) * isect.normal;
			reflect.direction = reflectDirection;
			rColor = traceRay(reflect, scene, depth + 1, maxDepth, samples, idx);
		}

		total /= glm::vec3(scene.lights.size());
		total = total * glm::vec3(1 - isect.objectHit->material->reflectivity) + 
				rColor * glm::vec3(isect.objectHit->material->reflectivity);
		total = glm::clamp(total, 0.f, 255.f);
		return total;
		break;
	}
	case MaterialType::PHONG: {
		glm::vec3 diffuse = glm::vec3(0);
		glm::vec3 specular = glm::vec3(0);
		for (auto &p : scene.lights) {
			Ray feeler = isect.raycast(p->getCastPoint(samples, idx));
			if (Intersection::getIntersection(feeler, scene).objectHit == p->geometry) {
                diffuse += color * glm::max(0.f, glm::dot(glm::normalize(isect.normal),
	                    			  glm::normalize(feeler.direction))) *
						glm::vec3(255);

				glm::vec3 r = glm::reflect(feeler.direction, isect.normal);
				specular += glm::vec3(glm::pow(glm::max(0.f,
					glm::dot(glm::normalize(r), glm::normalize(ray.direction))),
					isect.objectHit->material->n)) * glm::vec3(255);
			}
		}
		diffuse /= glm::vec3(scene.lights.size());
		specular /= glm::vec3(scene.lights.size());
		glm::vec3 reflective = glm::vec3(0);
		if (isect.objectHit->material->reflectivity > 0) {
			glm::vec3 reflectDirection = glm::reflect(ray.direction, isect.normal);
			Ray reflect;
			reflect.origin = isect.isectPoint + glm::vec3(0.01) * isect.normal;
			reflect.direction = reflectDirection;
			reflective = traceRay(reflect, scene, depth + 1, maxDepth, samples, idx);			
		}

		diffuse = diffuse * glm::vec3(1 - isect.objectHit->material->reflectivity) +
					reflective * glm::vec3(isect.objectHit->material->reflectivity);
		glm::vec3 total = diffuse * glm::vec3(1 - isect.objectHit->material->ks) + 
						  specular * glm::vec3(isect.objectHit->material->ks);
		return total;
		break;
	}
	case MaterialType::TRANSMISSIVE: {
		bool isOutside = glm::dot(ray.direction, isect.normal) < 0;
		glm::vec3 refractive = glm::vec3(0);
		float ior = isOutside ? isect.objectHit->material->iorOut / isect.objectHit->material->iorIn :
								isect.objectHit->material->iorIn / isect.objectHit->material->iorOut;
		float kr = fresnel(ray, isect.normal, ior);
		if (isOutside) {
			glm::vec3 refractDirection = glm::refract(ray.direction, isect.normal, ior);
			Ray refract;
			refract.origin = isect.isectPoint + glm::vec3(0.01) * -isect.normal;
			refract.direction = refractDirection;
			refractive = traceRay(refract, scene, depth + 1, maxDepth);
		} else {
			if (ior * ior * (1 - glm::pow(glm::dot(
				glm::normalize(ray.direction), glm::normalize(-isect.normal)), 2)) > 1) {
				// total internal reflection
				glm::vec3 reflectDirection = glm::reflect(-ray.direction, -isect.normal);
				Ray reflect;
				reflect.origin = isect.isectPoint + glm::vec3(0.01) * -isect.normal;
				reflect.direction = reflectDirection;
				refractive = traceRay(reflect, scene, depth + 1, maxDepth, samples, idx);
			} else {
				glm::vec3 refractDirection = glm::refract(ray.direction, -isect.normal, ior);
				Ray refract;
				refract.origin = isect.isectPoint + glm::vec3(0.01) * isect.normal;
				refract.direction = refractDirection;
				refractive = traceRay(refract, scene, depth + 1, maxDepth, samples, idx);
			}
		}
		glm::vec3 reflective = glm::vec3(0);
		if (kr > 0) {
			glm::vec3 reflectDirection = glm::reflect(ray.direction, isect.normal);
			Ray reflect;
			reflect.origin = isect.isectPoint + glm::vec3(0.01) * isect.normal;
			reflect.direction = reflectDirection;
			reflective = traceRay(reflect, scene, depth + 1, maxDepth, samples, idx);			
		}
		glm::vec3 total = reflective * glm::vec3(kr) +
						  refractive * glm::vec3(1 - kr);
		return total;
		break;
	}
	default:
		break;
	}

	return glm::vec3(0);

	// Iterate through all lights, and call isect.raycast(light position) to
	// get a light feeler ray. Calculate the intersection of each of these.
	// If the object hit is the light itself (not obstructed), determine
	// the color of the current pixel by calling
	// isect.objectHit->material.getColor(). Store this. If the object
	// is reflective or refractive, recursively call traceRay with a new
	// transformed ray.
}

float RaytraceEngine::fresnel(const Ray &ray, glm::vec3 normal, float ior) {
	float cosi = glm::clamp(glm::dot(ray.direction, normal), -1.f, 1.f);
	float etai = 1;
	float etat = ior;
	if (cosi > 0) {
		etai = etat;
		etat = 1;
	}
	float sint = etai / etat * glm::sqrt(glm::max(0.f, 1 - cosi * cosi));
	if (sint >= 1) {
		return 1;
	} else {
		float cost = glm::sqrt(glm::max(0.f, 1 - sint * sint));
		cosi = glm::abs(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return (Rs * Rs + Rp * Rp) / 2;
	}
}

QImage RaytraceEngine::generateAOPass(const Camera &camera, const Scene &scene,
									  int samples, float spread, float distance) {

	// Cast rays through each pixel. If the ray hits a geometry,
	// cast *samples* rays from the intersection point in a
	// cone (defined by spread, 1 is a hemisphere) oriented with
	// the normal of the intersection point. Count the number of
	// these samples that hit another geometry within *distance*.
	// Set the intensity of the pixel to be:
	// 				1 - (num_hit_samples / samples)
	// If the initial ray doesn't hit a geometry, make the pixel
	// completely transparent.
	// This resulting AO image can be blended (multiply) with
	// the rendered image to generate the desired effect.
	QImage ret = QImage(camera.width, camera.height, QImage::Format_RGB32);
	for (int i = 0; i < camera.width; ++i) {
		for (int j = 0; j < camera.height; ++j) {
			if (i == 304 && j == 208) {
				int k = 0;
			}
			Ray ray = camera.raycast(i, j);
			glm::vec4 color = traceAORay(ray, scene, samples, spread, distance);
			QRgb *line = (QRgb *) ret.scanLine(j);
			line += i;
			*line = qRgba(color.x, color.y, color.z, color.w);
		}
    }
	return ret;
}

// Uses consine weighted sampling to uniformly sample
// points on a hemisphere.
//glm::vec3 CosSampleHemisphere(glm::vec3 &sample) {
//    glm::vec2 square = Sample::warpDisk(sample);
//    float z =std::sqrt(std::max((Float)0, 1 - d.x * d.x - d.y * d.y));
//    return vec3(d.x, d.y, d.z);
//}

glm::vec4 RaytraceEngine::traceAORay(const Ray &ray, const Scene &scene,
								 int samples, float spread, float distance) {
	Intersection isect = Intersection::getIntersection(ray, scene);
	if (isect.objectHit == nullptr) { return glm::vec4(0); }

    int hitCount = 0;
    std::vector<glm::vec3> points;
    Sampler sampler;
    sampler.generateSamples(samples, points, Warp::HemiCos);
    sampler.transformSamples(isect, points);
    for (int i = 0; i < points.size(); ++i) {
      Ray ray = isect.raycast(points[i]);
      Intersection sampleIsect = Intersection::getIntersection(ray, scene);
      if (sampleIsect.t > 0 && sampleIsect.t < distance) {
          hitCount++;
      }
    }

	float intensity = hitCount / (float) samples;
	return glm::vec4(glm::vec3(255 * (1 - intensity)), 255);
}


void RaytraceEngine::writeColorToImage(QImage &img, int x, int y, glm::vec3 color) {
	img.setPixel(x, y, qRgb(color.x, color.y, color.z));
	emit updateGUI();
}

void RaytraceEngine::emitUpdateGUI() {
	emit updateGUI();
}
