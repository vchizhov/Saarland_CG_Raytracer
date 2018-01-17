#ifndef VCDR_RENDERER_HEADER
#define VCDR_RENDERER_HEADER

#include <rendering_comp/vcdr_util.h>
#include <thread>
#include <chrono>
#include <random>

namespace rt
{

	class Image;
	class VCDR_Renderer
	{
	private:
		void renderThread(uint threadIndex, Image& image, Rect<uint> rect);
	public:
		VCDR_Renderer(uint numThreads=0);
		~VCDR_Renderer();

		void render(Image& image);
	private:
		std::thread** threads;
		uint numThreads;
		std::mt19937** rndEngines;
		std::uniform_real_distribution<float>** rndDist;
		std::chrono::high_resolution_clock clock;
	};
}

#endif
