## Raytracer Design
### `scene/`
Describes the scene and the camera, along with utilities to load a scene (`JsonReader`). `JsonReader` reads in a valid scene file and creates a valid scene file (described as a scene graph).

### `scene/geometry/`
Describes the behavior of various primitives. Each primitive inherits `Geometry` and implements the `bool intersect(const Ray &ray, Intersection *intersection) const;` method, that returns whether or not the given ray intersects the primitive, and if so, populates the `Intersection` object with the geometry hit, the `t` value, the point of intersection, and the surface normal of the intersection point. The primitives include squareplane, cube, triangle, mesh, sphere, disk, cone, cylidner, paraboloid, hyperboloid.

### `scene/materials/`
Describes how rays interact with different materials. Contains member variables for storing textures, normal maps, reflectivity, and refractive indices of the material, as well as whether the material is Lambertian or Blinn-Phong.

### `scene/lights/`
Describes different models for lights, including point and area lights. A point light casts hard shadows, while an area light (when rendered with the appropriate number of samples) casts soft shadows.

### `raytracer/`
Contains the actual ray tracing code along with utility classes (`Ray` and `Intersection`). `RaytraceEngine` contains static methods to render a given scene. Has options to use SSAA and ambient occlusion. Also contains multithreading capability in the form of `RenderTasks` that will divide up the image into smaller tiles to render independently on separate threads.

### `acceleration/`
Describes how to create and traverse acceleration structures. Contains a bounding box class as well as a kd-tree.

### `sampler/`
An important part of a distribution ray tracer is proper sampling. The `Sampler` class is capable of generating stratified samples in a square grid, and can warp these samples to other spaces.

### `glm/`
Contains the `glm` library.

### `tinyobj/`
Contains the `tinyobj` library.
