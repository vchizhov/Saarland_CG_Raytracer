#ifndef CG1RAYTRACER_RENDERER_HEADER
#define CG1RAYTRACER_RENDERER_HEADER

#include <core/scalar.h>
#include <thread>
#include <chrono>
#include <core/random.h>
namespace rt {

class Image;
class Camera;
class Integrator;

class Renderer {
private:
	void test_render2_thread(Image& img, uint x0, uint x1, uint y0, uint y1);
	void render_thread(Image& img, uint x0, uint x1, uint y0, uint y1);
public:
    Renderer(Camera* cam, Integrator* integrator);
	~Renderer();
    void setSamples(uint samples);
    void render(Image& img);
    void test_render1(Image& img);
    void test_render2(Image& img);
private:
    Camera* cam;
    Integrator* integrator;
    uint samples;
	// number of threads of the system
	uint numThreads;
	// the threads
	std::thread** threads;
	// a clock for benchamrking
	std::chrono::high_resolution_clock clock;
};

}

#endif
