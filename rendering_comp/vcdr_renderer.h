#ifndef VCDR_RENDERER_HEADER
#define VCDR_RENDERER_HEADER

#include <rendering_comp/vcdr_util.h>
#include <rendering_comp/vcdr_world.h>
#include <rendering_comp/vcdr_camera.h>
#include <rendering_comp/vcdr_integrator.h>
#include <thread>
#include <chrono>
#include <random>

namespace rt
{

	struct Rnd
	{
	private:
		std::mt19937 eng;
		std::uniform_real_distribution<float> dist;
	public:
		Rnd(uint index) : eng(index), dist(std::uniform_real_distribution<float>(0.0f, 1.0f))
		{}

		float random()
		{
			return dist(eng);
		}
	};

	class Image;
	class VCDR_Renderer
	{
	private:
		void renderThread(uint threadIndex, Image& image, VCDR_World* world, VCDR_Camera* cam, VCDR_Integrator* integrator, Rect<uint> rect);
	public:
		VCDR_Renderer(uint samples = 1, uint timeSamples = 1, uint numThreads=0);
		~VCDR_Renderer();

		void render(Image& image, VCDR_World* world, VCDR_Camera* cam, VCDR_Integrator* integrator);
	private:
		std::thread** threads;
		uint numThreads;
		Rnd** rnd;
		std::chrono::high_resolution_clock clock;
		uint samples;
		float samplesDelta;
		uint timeSamples;
		float timeDelta;
	};
}

#endif
