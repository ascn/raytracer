#include <sampler/sampler.h>
#include <sampler/pcg32.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

Sampler::Sampler() {}

/**
 * Generates *samples* in (0, 1]^2 with a stratified pattern. Warps all
 * samples according to method, and stores these samples in vec.
 *
 * Warp::Hemi warps the samples to a hemisphere around (0, 0, 1)
 */
void Sampler::generateSamples(int samples, std::vector<glm::vec3> &vec, Warp method) {
	vec.clear();

	// Stratified sampling
	for (int i = 0; i < samples; ++i) {
		int n = glm::sqrt((float) samples) + 0.5;
		float inv = 1.f / n;
		int x = i / n;
		int y = i % n;
		glm::vec2 pt = glm::vec2(x + random.nextFloat(), y + random.nextFloat()) * glm::vec2(inv);
		glm::vec3 warped;
		switch (method) {
		case Warp::Square:
			warped = glm::vec3(pt, 0);
			break;
		case Warp::Disk:
			warped = Sampler::warpDisk(pt);
			break;
		case Warp::Sphere:
			break;
		case Warp::HemiCos:
			warped = Sampler::warpDisk(pt);
			warped.z = glm::sqrt(glm::max(0.f, 1 - warped.x * warped.x - warped.y * warped.y));
			break;
		}
		vec.push_back(warped);
	}
}

glm::vec3 Sampler::warpDisk(const glm::vec2 &sample) {
	float const PI_4 = 0.78539816;
	glm::vec2 square = (sample - glm::vec2(0.5)) * glm::vec2(2);
	float phi, r;
	if (square.x > -square.y) {
		if (square.x > square.y) {
			r = square.x;
			phi = PI_4 * (square.y / square.x);
		} else {
			r = square.y;
			phi = PI_4 * (2 - (square.x / square.y));
		}
	} else {
		if (square.x < square.y) {
			r = -square.x;
			phi = PI_4 * (4 + (square.y / square.x));
		} else {
			r = -square.y;
			if (square.y != 0) {
				phi = PI_4 * (6 - (square.x / square.y));
			} else {
				phi = 0;
			}
		}
	}
	return glm::vec3(r * glm::cos(phi), r * glm::sin(phi), 0);
}

void Sampler::transformSamples(Intersection isect, std::vector<glm::vec3> &points) {
  
    for (int idx = 0; i < points.size(); ++idx) {

    	points[idx] -= glm::vec3(0.5, 0.5, 0.5);
   		glm::vec3 normal = isect.objectHit->transform.invTransform * glm::vec4(isect.normal, 0);
	    glm::vec3 tangent = glm::normalize(glm::cross(glm::vec3(0, 1, 0), normalInit));
        glm::vec3 bitangent = glm::normalize(glm::cross(normalInit, tangent));
        glm::mat3 trans = glm::mat4(glm::vec4(tangent, 0), glm::vec4(bitangent, 0),
                                    glm::vec4(normal, 0), glm::vec4(0, 0, 0, 1));
        points[idx] =  isect.isectPoint + trans * points[idx];
        points[idx] += glm::vec3(0.1) * normal
	}
}
