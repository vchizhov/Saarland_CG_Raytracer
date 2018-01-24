#include <rendering_comp/vcdr_renderer.h>
#include <core/image.h>
#include <iostream>
#include <rendering_comp/vcdr_util.h>
#include <math.h>
#include <core/vector.h>
#include <rendering_comp/vcdr_procedural.h>
#include <core/random.h>

namespace rt
{

	// renders an fbm without axial bias mitigation
	RGBColor renderScene001(uint x, uint y, uint resX, uint resY)
	{
		float sca = 25.0f;
		float x1 = sca * (float(x) / float(resX) - 0.5f);
		float y1 = sca * (float(y) / float(resY) - 0.5f);
		float v = FBM2D<rt::ValueNoise2D<rt::hash12_iq0>>::fbmc<false>(vec2(x1, y1), 4, 0.5f, 2.0f);
		float ns = clamp(0.0f, 1.0f, 0.5f*v);
		return RGBColor::rep(ns);
	}

	// renders an fbm with axial bias mitigation
	RGBColor renderScene002(uint x, uint y, uint resX, uint resY)
	{
		float sca = 25.0f;
		float x1 = sca * (float(x) / float(resX) - 0.5f);
		float y1 = sca * (float(y) / float(resY) - 0.5f);
		float v = FBM2D<rt::ValueNoise2D<rt::hash12_iq0>>::fbm<false>(vec2(x1, y1), 4, 0.5f, 2.0f);
		float ns = clamp(0.0f, 1.0f, 0.5f*v);
		return RGBColor::rep(ns);
	}

	// renders the normals of an fbm with bias mitigation
	RGBColor renderScene003(uint x, uint y, uint resX, uint resY)
	{
		float sca = 25.0f;
		float x1 = sca * (float(x) / float(resX) - 0.5f);
		float y1 = sca * (float(y) / float(resY) - 0.5f);
		vec3 v = FBM2D<rt::ValueNoise2D<rt::hash12_iq0>>::fbmD<false>(vec2(x1, y1), 4, 0.5f, 2.0f);
		vec3 n = 0.5f*normalize(vec3(-v.y, 1.0f, -v.z))+0.5f;
		return RGBColor(n.x, n.y, n.z);
	}

	RGBColor renderScene(uint x, uint y, uint resX, uint resY)
	{
		float sca = 25.0f;
		float x1 = sca * (float(x) / float(resX) - 0.5f);
		float y1 = sca * (float(y) / float(resY) - 0.5f);
		vec3 v = FBM2D<rt::ValueNoise2D<rt::hash12_iq0>>::fbmD<false>(vec2(x1, y1), 4, 0.5f, 2.0f);
		vec3 n = 0.5f*normalize(vec3(-v.y, 1.0f, -v.z)) + 0.5f;
		return RGBColor(n.x, n.y, n.z);
	}


	VCDR_Renderer::VCDR_Renderer(uint samples, uint timeSamples, uint numThreads)
		: samples(samples), timeSamples(timeSamples), samplesDelta(1.0f/float(samples)), timeDelta(1.0f/float(timeSamples))
	{
		// if numThreads==0 (the default) - query the maximum number of threads on the machine
		this->numThreads = (numThreads!=0) ? numThreads : std::thread::hardware_concurrency();
		threads = new std::thread*[this->numThreads];
		rnd = new Rnd*[this->numThreads];
		for (uint i = 0; i < this->numThreads; ++i)
		{
			threads[i] = nullptr;
			rnd[i] = new Rnd(i);
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
			delete rnd[i];
		}
		delete[] threads;
		delete[] rnd;
	}

	void VCDR_Renderer::renderThread(uint threadIndex, Image& image, VCDR_World* world, VCDR_Camera* cam, VCDR_Integrator* integrator, Rect<uint> rect)
	{
		for (uint y = rect.y0; y < rect.y1; ++y)
		{
			for (uint x = rect.x0; x < rect.x1; ++x)
			{
				vec3 res = vec3(0.0f);
				// integrate over time (camera movement)
				for (uint st = 0; st < timeSamples; ++st)
				{
					// jittered sampling
					for (uint sy = 0; sy < samples; ++sy)
					{
						for (uint sx = 0; sx < samples; ++sx)
						{
							float t = (st + random())*timeDelta;
							float ndcX = 2.0f * (float(x) + samplesDelta*(sx + random())) / float(image.width()) - 1.0f;
							float ndcY = -2.0f * (float(y) + samplesDelta*(sy + random())) / float(image.height()) + 1.0f;
							vec3 ro = cam->origin() + t*vec3(0.0f, 0.0f, 0.1f);
							vec3 rd = cam->dir(ndcX, ndcY);
							res += integrator->getColor(ro, rd, world);
						}
					}
				}
				// normalize
				res = res / sqr(float(samples)) / float(timeSamples);
				// ~ gamma correction (1/2 is less correct but faster than 1/2.2)
				res = sqrt(res);
				image(x, y) = RGBColor(res.x, res.y, res.z);
			}
		}
	}

	void VCDR_Renderer::render(Image& image, VCDR_World* world, VCDR_Camera* cam, VCDR_Integrator* integrator)
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
			threads[j] = new std::thread(&VCDR_Renderer::renderThread, this, j, std::ref(image), world, cam, integrator, Rect<uint>(0, j*deltaY, image.width(), (j + 1)*deltaY));
		threads[j] = new std::thread(&VCDR_Renderer::renderThread, this, j, std::ref(image), world, cam, integrator, Rect<uint>(0, j*deltaY, image.width(), image.height()));

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