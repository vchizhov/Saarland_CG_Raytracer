#include <rendering_comp/vcdr_renderer.h>
#include <core/image.h>
#include <iostream>
#include <rendering_comp/vcdr_util.h>
#include <math.h>
#include <core/vector.h>

namespace rt
{

	// Author iq - doesn't seem to be good around 0
	float iq0_hash1(float x, float y)
	{
		float px = 50.0f*fract(x*0.3183099f);
		float py = 50.0f*fract(y*0.3183099f);
		return fract(px*py*(px + py));
	}

	// Author @patriciogv - 2015
	// https://thebookofshaders.com/13/
	float particio0_hash1(float x, float y)
	{
		return fract(sin(x*12.9898f+y*78.233f)*43758.5453123f);
	}

	// the hash function from the CG1 framework (called noise in the framework)
	float cg_hash3(int x, int y, int z) {
		int n = x + y * 57 + z * 997;
		n = (n << 13) ^ n;
		return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}

#define hash(x,y) iq0_hash1(x,y)
	// 2d value noise - billinear interpolation
	float valueNoise(float x, float y)
	{
		float px = floorf(x);
		float py = floorf(y);
		float wx = fract(x);
		float wy = fract(y);
		// quintic smoothstep - smoother(0 1st and 2nd order deriv at x=0,x=1) - suggested by Ken Perlin: https://en.wikipedia.org/wiki/Smoothstep
		float ux = wx*wx*wx*(wx*(wx*6.0f - 15.0f) + 10.0f);
		float uy = wy*wy*wy*(wy*(wy*6.0f - 15.0f) + 10.0f);

		float a = hash(px, py);
		float b = hash(px + 1.0f, py);
		float c = hash(px, py + 1.0f);
		float d = hash(px + 1.0f, py + 1.0f);

		// perform billinear interpolation
		float bilinear = a + (b - a)*ux + (c - a)*uy + (a - b - c + d)*ux*uy;
		// remap to [-1.0,1.0]
		return bilinear;
	}

	// fractal Brownian motion
	float fbm(float x, float y)
	{
		float sum = 0.0f;
		float amplitude = 1.0f;
		float frequency = 1.0f;
		const int octaves = 5;
		for (int i = 0; i < octaves; ++i)
		{
			sum += amplitude*valueNoise(frequency*x, frequency*y);
			amplitude *= 0.5f;
			frequency *= 2.0f;
		}

		return sum;
	}

	// improved fractal Brownian motion
	float fbm2(float x, float y)
	{
		float sum = 0.0f;
		float amplitude = 1.0f;
		float frequency = 1.0f;
		const int octaves = 8;
		float shift = 100.0f;
		x *= frequency;
		y *= frequency;
		for (int i = 0; i < octaves; ++i)
		{
			sum += amplitude*valueNoise(x, y);
			amplitude *= 0.5f;
			float temp = (0.8f*y + -0.6f*x);
			x = (0.8f*x + 0.6f*y)*2.0f;
			y = temp*2.0f;
		}

		return sum;
	}

	RGBColor renderScene(uint x, uint y, uint resX, uint resY)
	{
		float sca = 5.0f;
		float x1 = sca * (float(x) / float(resX));
		float y1 = sca * (float(y) / float(resY));
		float ns = clamp(0.0f, 1.0f, 0.1f + 0.5f + 0.5f*fbm2(x1, y1));
		return RGBColor::rep(ns);
	}

	RGBColor trace(Vector o, Vector rd)
	{
		float delta = 0.1f;
		const float mint = 0.0f;
		const float maxt = 100.0f;
		const float sm = 2.0f;
		const float scale = 0.5f/sm;
		const float height = 2.0f*sm;
		float ly = 0.0f;
		float lh = 0.0f;
		for (float t = 0; t < maxt; t += delta)
		{
			Vector p = o + t*rd;
			float h = height*clamp(0.0f, 1.0f, 0.1f + fbm2(p.x*scale, p.z*scale));
			if (p.y < h)
			{
				float resT = t-delta + delta*(lh-ly)/(p.y-ly-h+lh);
				p = o + resT*rd;
				float ns = clamp(0.0f, 1.0f, fbm2(p.x*scale, p.z*scale));
				return RGBColor(0.0f, h/height, 0.0f);
				delta = t*0.1f;
			}
			ly = p.y;
			lh = h;
		}
		return RGBColor::rep(0.0f);
	}

	RGBColor renderScene2(uint x, uint y, uint resX, uint resY)
	{
		float sca = 5.0f;
		float ndcX = 2.0f*(float(x) / float(resX)) - 1.0f;
		float ndcY = -2.0f*(float(y) / float(resY)) + 1.0f;
		Vector origin = Vector(0.0f, 5.0f, 0.0f);
		float fov = 2.0f*pi / 3.0f;
		float scaleX = tanf(fov / 2.0f);
		Vector right = Vector(1.0f, 0.0f, 0.0f)*scaleX;
		Vector forward = Vector(0.0f, 0.0f, 1.0f);
		float aspectRatio = float(resY) / float(resX);
		Vector up = Vector(0.0f, 1.0f, 0.0f)*scaleX*aspectRatio;
		Vector rd = (forward + ndcX*right + ndcY*up).normalize();
		return trace(origin, rd);
	}


	VCDR_Renderer::VCDR_Renderer(uint numThreads)
	{
		// if numThreads==0 (the default) - query the maximum number of threads on the machine
		this->numThreads = (numThreads!=0) ? numThreads : std::thread::hardware_concurrency();
		threads = new std::thread*[this->numThreads];
		rndEngines = new std::mt19937*[this->numThreads];
		rndDist = new std::uniform_real_distribution<float>*[this->numThreads];
		for (uint i = 0; i < this->numThreads; ++i)
		{
			threads[i] = nullptr;
			rndEngines[i] = new std::mt19937(i);
			rndDist[i] = new std::uniform_real_distribution<float>();
		}
	}

	VCDR_Renderer::~VCDR_Renderer()
	{
		// clean-up the threads
		for (uint i = 0; i < numThreads; ++i)
		{
			if (threads[i] != nullptr && threads[i]->joinable())
				threads[i]->join();
			delete threads[i];
			delete rndDist[i];
			delete rndEngines[i];
		}
		delete[] threads;
		delete[] rndDist;
		delete[] rndEngines;
	}

	void VCDR_Renderer::renderThread(uint threadIndex, Image& image, Rect<uint> rect)
	{
		for (uint y = rect.y0; y < rect.y1; ++y)
		{
			for (uint x = rect.x0; x < rect.x1; ++x)
			{
				image(x, y) = renderScene2(x, y, image.width(), image.height());
			}
		}
	}

	void VCDR_Renderer::render(Image& image)
	{
		// record current time
		auto beginRenderTimeStamp = clock.now();

		// calculate the height of the region that each thread except the last one gets
		uint deltaY = image.height() / numThreads;
		uint maxThreads;
		if (image.height() > numThreads)
		{
			deltaY = image.height() / numThreads;
			maxThreads = numThreads;
		}
		else
		{
			deltaY = 1;
			maxThreads = image.height();
		}

		// fire off the threads
		uint j = 0;
		for (;j < maxThreads - 1; ++j)
			threads[j] = new std::thread(&VCDR_Renderer::renderThread, this, j, std::ref(image), Rect<uint>(0, j*deltaY, image.width(), (j + 1)*deltaY));
		threads[j] = new std::thread(&VCDR_Renderer::renderThread, this, j, std::ref(image), Rect<uint>(0, j*deltaY, image.width(), image.height()));

		// wait for each thread to finish its job
		for (uint i = 0; i < maxThreads; ++i)
		{
			threads[i]->join();
			delete threads[i];
			threads[i] = nullptr;
		}


		// print out the elapsed time
		std::chrono::duration<double> elapsedTime = clock.now() - beginRenderTimeStamp;
		std::cout << "Rendering took: " << elapsedTime.count() << " seconds.\n";
	}
}