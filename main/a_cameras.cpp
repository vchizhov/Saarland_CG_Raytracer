#include <core/image.h>
#include <core/scalar.h>
#include <core/julia.h>
#include <rt/ray.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/orthographic.h>
#include <iostream>
#include <rt/renderer.h>
#include <rt/cameras/fisheye.h>
#include <rt/cameras/environment.h>

using namespace rt;

float a2computeWeight(float fx, float fy, const Point& c, float div) {
    Point v(fx, fy, 0.0f);
    int numIter = julia(v, c);
    return numIter/(numIter+div);
}

RGBColor a2computeColor(const Ray& r) {
    float theta = asin(r.d.z)/pi*2;
    float phi = atan2(r.d.y,r.d.x)/pi;
    float ofx = absfractional((r.o.x+1.0f)/2.0f)*2.0f-1.0f;
    float ofy = absfractional((r.o.y+1.0f)/2.0f)*2.0f-1.0f;
    RGBColor color = RGBColor::rep(0.0f);
    color = color + a2computeWeight(phi, theta, Point(-0.8f, 0.156f, 0.0f), 64.0f) * RGBColor(0.8f, 0.8f, 1.0f);
    color = color + a2computeWeight(phi, theta, Point(-0.6f, 0.2f, 0.0f), 64.0f)*0.2f * RGBColor(0.5f, 0.5f, -0.2f);
    color = color + a2computeWeight(ofy, ofx, Point(0.285f, 0.0f, 0.0f), 64.0f) * RGBColor(0.4f, 0.5f, 0.6f);
    color = RGBColor::rep(1.0f) - color;
    if (absfractional(theta/(2*pi)*90)<0.03) color = RGBColor(0.9f,0.5f,0.5f)*0.7f;
    if (absfractional(phi/(2*pi)*90)<0.03) color = RGBColor(0.9f,0.9f,0.5f)*0.7f;
    return color;
}


void a_cameras() {
    Image img(800, 800);
    Image low(128, 128);

    PerspectiveCamera pcam(Point(0, 0, 0), Vector(1.0f, 0, 0.1f), Vector(0, 0, 1), pi/3.0f, pi/3.0f);
    Renderer r1(&pcam,0);
    r1.test_render2(img);
    r1.test_render2(low);
    img.writePNG("a1-2.png");
    low.writePNG("a1-2-low.png");

    PerspectiveCamera pcam2(Point(0, 0, 0), Vector(0.5f, 0.5f, 0.3f), Vector(0, 0, 1), pi * 0.9f, pi * 0.9f);
    Renderer r12(&pcam2,0);
    r12.test_render2(img);
    img.writePNG("a1-3.png");

    OrthographicCamera ocam(Point(0, 0, 0), Vector(0.1f, 0.1f, 1), Vector(0.2f, 1.0f, 0.2f), 10.f, 10.f);
    Renderer r2(&ocam,0);
    r2.test_render2(img);
    img.writePNG("a1-4.png");

	FisheyeCamera fecam(Point(0, 0, 0), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), FISHEYE_EQUIDISTANT, 1.0f, 1.0f, Vector(1.0f, 1.0f, 1.0f));
	Renderer rfe(&fecam, 0);
	rfe.test_render2(img);
	img.writePNG("fisheye_equidistant.png");

	FisheyeCamera fscam(Point(0, 0, 0), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), FISHEYE_STEREOGRAPHIC, 1.0f, 2.0f, Vector(1.0f, 1.0f, 1.0f));
	Renderer rfs(&fscam, 0);
	rfs.test_render2(img);
	img.writePNG("fisheye_stereographic.png");

	FisheyeCamera focam(Point(0, 0, 0), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), FISHEYE_ORTHOGRAPHIC, 1.0f, 2.0f, Vector(1.0f, 1.0f, 1.0f));
	Renderer rfo(&focam, 0);
	rfo.test_render2(img);
	img.writePNG("fisheye_orthographic.png");

	EnvironmentCamera ecam(Point(0, 0, 0), Vector(0.4f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), 2.0f*pi, pi, Vector(1.0f, 1.0f, 1.0f));
	Renderer re(&ecam, 0);
	re.test_render2(img);
	img.writePNG("environment.png");

	EnvironmentCamera ecam2(Point(0, 0, 0), Vector(0.4f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), 2.0f*pi, pi, Vector(2.0f, 1.0f, 1.0f));
	Renderer re2(&ecam2, 0);
	re2.test_render2(img);
	img.writePNG("environment_spheroid.png");

}
