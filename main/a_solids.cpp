
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/sphere.h>
#include <rt/solids/infiniteplane.h>
#include <rt/solids/triangle.h>
#include <rt/solids/quad.h>
#include <rt/solids/disc.h>
#include <rt/solids/aabox.h>
#include <rt/solids/quadric.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/castingdist.h>
#include <rt/renderer.h>

using namespace rt;

void a_solids() {
    Image img(640, 480);

    SimpleGroup* scene = new SimpleGroup();
    scene->add(new Sphere(Point(-2.f,  1.7f,  0), 2   , nullptr, nullptr));
    scene->add(new Sphere(Point(1,    -1,     1), 2.2f, nullptr, nullptr));
    scene->add(new Sphere(Point(3.f,   0.8f, -2), 2, nullptr, nullptr));

    scene->add(new InfinitePlane(Point(0,- 1, 0), Vector(0, 1, 0), nullptr, nullptr));

    scene->add(new Triangle(Point(-2, 3.7f, 0), Point(1, 2,  1), Point( 3, 2.8f, -2), nullptr, nullptr));
    scene->add(new Triangle(Point(3,  2,    3), Point(3, 2, -3), Point(-3, 2   , -3), nullptr, nullptr));
	scene->add(new Quad(Point(1,  -0.9f,    4.5f), Vector(-2, 0, 0), Vector(0, 0.1f, -2), nullptr, nullptr));

    scene->add(new Disc(Point(-3, -0.75f, 1.5f), Vector(0, 0.5f, 0.5f), 1.5f, nullptr, nullptr));

    scene->add(new AABox(Point(2,1.5f,-0.5f), Point(3,2.5f,2.5f), nullptr, nullptr));

    World world;
    world.scene = scene;

    PerspectiveCamera cam(Point(0, 0, 10), Vector(0, 0, -1), Vector(0, 1, 0), pi/4, pi/3);
    

    RayCastingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.render(img);
    img.writePNG("a2-1.png");

    RayCastingDistIntegrator integratorb(&world, RGBColor(1.0f,0.2f,0.0f), 4.0f, RGBColor(0.2f,1.0f,0.0f), 12.0f);
    Renderer engineb(&cam, &integratorb);
    engineb.render(img);
    img.writePNG("a2-2.png");

	
	// Quadrics
	PerspectiveCamera cam2(Point(0.0f, 0.0f, 5.0f), Vector(0.0f, 0.0f, -1.0f), Vector(0.0f, 1.0f, 0.0f), pi / 4, pi / 3);


	// Spheroid, I just streteched a sphere along x
	Primitive* spheroid = new Quadric(0.5, 1, 1, 0, 0, 0, 0, 0, 0, -1.0f, nullptr, nullptr);
	world.scene = spheroid;

	RayCastingIntegrator intS(&world);
	Renderer engineS(&cam2, &intS);
	engineS.render(img);
	img.writePNG("quadric_spheroid.png");


	// Elliptic paraboloid
	Primitive* ellipticParaboloid = new Quadric(1, 0, 1, 0, 0, 0, 0, -1, 0, 0.0f, nullptr, nullptr);
	world.scene = ellipticParaboloid;

	RayCastingIntegrator intEP(&world);
	Renderer engineEP(&cam2, &intEP);
	engineEP.render(img);
	img.writePNG("quadric_elliptic_paraboloid.png");


	// Hyperbolic paraboloid (saddle)
	Primitive* hyperbolicParaboloid = new Quadric(1, 0, -1, 0, 0, 0, 0, -1, 0, 0.0f, nullptr, nullptr);
	world.scene = hyperbolicParaboloid;

	RayCastingIntegrator intHP(&world);
	Renderer engineHP(&cam2, &intHP);
	engineHP.render(img);
	img.writePNG("quadric_hyperbolic_paraboloid.png");


	// one sheet hyperboloid
	Primitive* hyperboloid1 = new Quadric(1, -1, 1, 0, 0, 0, 0, 0, 0, -1.0f, nullptr, nullptr);
	world.scene = hyperboloid1;

	RayCastingIntegrator intH1(&world);
	Renderer engineH1(&cam2, &intH1);
	engineH1.render(img);
	img.writePNG("quadric_hyperboloid1.png");

	// two sheets hyperboloid
	Primitive* hyperboloid2 = new Quadric(1, -1, 1, 0, 0, 0, 0, 0, 0, 1.0f, nullptr, nullptr);
	world.scene = hyperboloid2;

	RayCastingIntegrator intH2(&world);
	Renderer engineH2(&cam2, &intH2);
	engineH2.render(img);
	img.writePNG("quadric_hyperboloid2.png");

	//degenerate cases:

	//elliptic cone
	Primitive* ellipticCone = new Quadric(0.5f, -1, 2.0f, 0, 0, 0, 0, 0, 0, 0.0f, nullptr, nullptr);
	world.scene = ellipticCone;

	RayCastingIntegrator intEC(&world);
	Renderer engineEC(&cam2, &intEC);
	engineEC.render(img);
	img.writePNG("quadric_elliptic_cone.png");

	//elliptic cylinder
	Primitive* ellipticCylinder = new Quadric(0.5f, 0.0f, 2.0f, 0, 0, 0, 0, 0, 0, -1.0f, nullptr, nullptr);
	world.scene = ellipticCylinder;

	RayCastingIntegrator intECY(&world);
	Renderer engineECY(&cam2, &intECY);
	engineECY.render(img);
	img.writePNG("quadric_elliptic_cylinder.png");

	//hyperbolic cylinder
	Primitive* hyperbolicCylinder = new Quadric(1.0f, 0.0f, -1.0f, 0, 0, 0, 0, 0, 0, -1.0f, nullptr, nullptr);
	world.scene = hyperbolicCylinder;

	RayCastingIntegrator intHC(&world);
	Renderer engineHC(&cam2, &intHC);
	engineHC.render(img);
	img.writePNG("quadric_hyperbolic_cylinder.png");


	//parabolic cylinder
	Primitive* parabolicCylinder = new Quadric(1.0f, 0.0f, 0.0f, 0, 0, 0, 0, 0, 2.0f, 0.0f, nullptr, nullptr);
	world.scene = parabolicCylinder;

	RayCastingIntegrator intPC(&world);
	Renderer enginePC(&cam2, &intHC);
	enginePC.render(img);
	img.writePNG("quadric_paraolic_cylinder.png");

}
