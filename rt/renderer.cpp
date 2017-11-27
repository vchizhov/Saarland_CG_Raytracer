#include <rt/renderer.h>
#include <rt/ray.h> // Point, Vector, Ray
#include <core/image.h> //RGBColor
#include <rt/cameras/camera.h> //Camera
#include <rt/integrators/integrator.h>
#include <iostream>

float a1computeWeight(float fx, float fy, const rt::Point& c, float div);
rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);
float a2computeWeight(float fx, float fy, const rt:: Point& c, float div);
rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt
{
	Renderer::Renderer(Camera* cam, Integrator* integrator)
		: cam(cam), integrator(integrator)
	{
		numThreads = std::thread::hardware_concurrency();
		threads = new std::thread*[numThreads];
		for (uint i = 0; i < numThreads; ++i)
			threads[i] = nullptr;
	}

	Renderer::~Renderer()
	{
		//clean-up the threads
		for (uint i = 0; i < numThreads; ++i)
		{
			if(threads[i]!=nullptr && threads[i]->joinable())
				threads[i]->join();
			delete threads[i];
		}
		delete[] threads;
	}

	void setSamples(uint samples)
	{

	}

	void Renderer::render_thread(Image& img, uint x0, uint x1, uint y0, uint y1)
	{
		for (uint y = y0; y < y1; ++y)
		{
			//[0,resY) -> [1,-1]
			// a*0 + b = 1 => b = 1
			// a*resY + b = -1 => a = -2/resY
			// the 0.5 offset is so that we pick the center of the pixel
			float ndcY = -2.0f*(float(y) + 0.5f) / float(img.height()) + 1.0f;
			for (uint x = x0; x < x1; ++x)
			{
				//[0,resX) -> [-1, 1]
				// a*0 + b = -1 => b = -1
				// a*resX + b = 1 => a = 2/resX
				// the 0.5 offset is so that we pick the center of the pixel
				float ndcX = 2.0f*(float(x) + 0.5f) / float(img.width()) - 1.0f;
				Ray ray = cam->getPrimaryRay(ndcX, ndcY);
				if (ray.d != Vector::rep(0.0f))
					img(x, y) = integrator->getRadiance(ray);
				else
					img(x, y) = RGBColor(0.0f, 0.0f, 0.0f);
			}
		}
	}

	void Renderer::render(Image& img)
	{
		// record current time
		auto beginRenderTimeStamp = clock.now();

		// calculate the height of the region each thread excep the last one gets
		uint deltaY = img.height() / numThreads;
		uint maxThreads;
		if (img.height() > numThreads)
		{
			deltaY = img.height() / numThreads;
			maxThreads = numThreads;
		}
		else
		{
			deltaY = 1;
			maxThreads = img.height();
		}

		// fire of the threads
		for (uint i = 0; i < maxThreads - 1; ++i)
			threads[i] = new std::thread(&Renderer::render_thread, this, std::ref(img), 0, img.width(), i*deltaY, (i + 1)*deltaY);
		threads[maxThreads - 1] = new std::thread(&Renderer::render_thread, this, std::ref(img), 0, img.width(), (numThreads - 1)*deltaY, img.height());

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

	void Renderer::test_render1(Image& img)
	{
		for (uint y = 0; y < img.height(); ++y)
		{
			for (uint x = 0; x < img.width(); ++x)
			{
				img(x, y) = a1computeColor(x, y, img.width(), img.height());
			}
		}
	}

	void Renderer::test_render2_thread(Image& img, uint x0, uint x1, uint y0, uint y1)
	{
		for (uint y = y0; y < y1; ++y)
		{
			//[0,resY) -> [1,-1]
			// a*0 + b = 1 => b = 1
			// a*resY + b = -1 => a = -2/resY
			// the 0.5 offset is so that we pick the center of the pixel
			float ndcY = -2.0f*(float(y) + 0.5f) / float(img.height()) + 1.0f;
			for (uint x = x0; x < x1; ++x)
			{
				//[0,resX) -> [-1, 1]
				// a*0 + b = -1 => b = -1
				// a*resX + b = 1 => a = 2/resX
				// the 0.5 offset is so that we pick the center of the pixel
				float ndcX = 2.0f*(float(x) + 0.5f) / float(img.width()) - 1.0f;
				Ray ray = cam->getPrimaryRay(ndcX, ndcY);
				if (ray.d != Vector::rep(0.0f))
					img(x, y) = a2computeColor(ray);
				else
					img(x, y) = RGBColor(0.0f, 0.0f, 0.0f);
			}
		}
	}

	void Renderer::test_render2(Image& img)
	{
		// record current time
		auto beginRenderTimeStamp = clock.now();

		// calculate the height of the region each thread excep the last one gets
		uint deltaY = img.height() / numThreads;
		uint maxThreads;
		if (img.height() > numThreads)
		{
			deltaY = img.height() / numThreads;
			maxThreads = numThreads;
		}
		else
		{
			deltaY = 1;
			maxThreads = img.height();
		}

		// fire of the threads
		for (uint i = 0; i < maxThreads-1; ++i)
			threads[i] = new std::thread(&Renderer::test_render2_thread, this, std::ref(img), 0, img.width(), i*deltaY, (i + 1)*deltaY);
		threads[maxThreads-1] = new std::thread(&Renderer::test_render2_thread, this, std::ref(img), 0, img.width(), (numThreads-1)*deltaY, img.height());

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