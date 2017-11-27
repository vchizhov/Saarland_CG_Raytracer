
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/groups/kdtree.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/groups/median_kdtree.h>
#include <rt/groups/sah_kdtree.h>
#include <rt/groups/simplegroup.h>
using namespace rt;

void a_indexing() {
    Image img(800,600);

    KDTree* scene = new SAHKDTree();
    scene->add(new Sphere(Point(2.5f,  .5f,  -1), 0.5  , nullptr, nullptr));
    scene->add(new Sphere(Point(2.5f,  -1.f,  -1), 0.5, nullptr, nullptr));
    scene->add(new Sphere(Point(4.5f,  .5f,  -1), 0.5 , nullptr, nullptr));
	
    loadOBJ(scene, "models/", "cow.obj");
	
	std::cout << "Build started:\n";
	std::chrono::high_resolution_clock clock;
	auto timeStamp = clock.now();
	scene->rebuildIndex();
	std::chrono::duration<double> dur = clock.now() - timeStamp;
	std::cout << "Build took: " << dur.count() << " seconds.\n";
    World world;
    world.scene = scene;

    PerspectiveCamera cam1(Point(-8.85f, -7.85f, 7.0f), Vector(1.0f,1.0f,-0.6f), Vector(0, 0, 1), pi/8, pi/6);
    PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f)-Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi/8, pi/6);
	//PerspectiveCamera cam1(Point(-0.0f, 8.0f, 50.0f), Vector(0.0f, 0.0f, -1.0f), Vector(0.0, 1.0, 0.0), pi / 8, pi / 6);
	RayCastingIntegrator integrator(&world);

	
    Renderer engine1(&cam1, &integrator);
    engine1.render(img);
    img.writePNG("a3-1.png");

	
    Renderer engine2(&cam2, &integrator);
    engine2.render(img);
    img.writePNG("a3-2.png");
	
}
