# raytracer

## `scene/`
Describes the scene and the camera, along with utilities to load a scene (`JsonReader`). `JsonReader` reads in a valid scene file and creates a valid scene file (described as a scene graph).

## `scene/geometry/`
Describes the behavior of various primitives. Each primitive inherits `Geometry` and implements the `bool intersect(const Ray &ray, Intersection *intersection) const;` method, that returns whether or not the given ray intersects the primitive, and if so, populates the `Intersection` object with the geometry hit, the `t` value, the point of intersection, and the surface normal of the intersection point.

## `scene/materials/`
Describes how rays interact with different materials. Contains member variables for storing textures, normal maps, reflectivity, and refractive indices of the material, as well as whether the material is Lambertian or Blinn-Phong.

## `raytracer/`
Contains the actual ray tracing code along with utility classes (`Ray` and `Intersection`). `RaytraceEngine` contains static methods to render a given scene. Has options to use SSAA and ambient occlusion. Also contains multithreading capability in the form of `RenderTasks` that will divide up the image into smaller tiles to render independently on separate threads.

## `acceleration/`
Describes how to create and traverse acceleration structures (likely octrees).

## `glm/`
Contains the `glm` library.

## `tinyobj/`
Contains the `tinyobj` library.

### Milestone 1
- Scene graphs
- JSONReader

AC:
- `Ray` and `Intersection` classes
- `Camera` class
- Raycasting (normal shading)
- Primitives
	- Cube
	- Sphere
	- Square Plane
	- Triangle (Mesh)

Sample Renders:

### Milestone 2
- Recursive raytracing for:
	- Reflections, refractions, and translucency
- Lambertian and Blinn-Phong shading
- Shadows
- Textures
- Normal maps

### Milestone 3
Planned features:
- SSAA via jittered sampling
- Ambient occlusion
	- Saves AO pass in separate image
	- Parameters:
		- Number of samples
		- Distance rays travel
		- Ray spread
- Additional shape primitives
	- Cylinders
	- Disks
	- Cones
	- Paraboloids
	- Hyperboloids