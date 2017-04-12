#include <scene/geometry/geometry.h>
#include <scene/geometry/sphere.h>
#include <scene/camera.h>

#include <scene/scene.h>

Scene::Scene() {

}

void Scene::reset() {
	for (auto &p : primitives) { delete(p);	}
	for (auto &p : lights) { delete(p); }
	for (auto &p : materials) { delete(p); }
	primitives.clear();
	lights.clear();
	materials.clear();
	materialsMap.clear();
}
