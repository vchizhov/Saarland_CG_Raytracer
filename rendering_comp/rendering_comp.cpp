#include <rendering_comp/vcdr_renderer.h>
#include <rendering_comp/vcdr_fbm_terrain.h>
#include <rendering_comp/vcdr_lake.h>
#include <rendering_comp/vcdr_clouds.h>
#include <rendering_comp/vcdr_cloud.h>
#include <rendering_comp/vcdr_camera_perspective.h>
#include <rendering_comp/vcdr_integrator_default.h>
#include <rendering_comp/vcdr_background_sky.h>
#include <core/image.h>

using namespace rt;

void rs005()
{
	uint width = 480;
	uint height = 270; 
	float aspectRatio = float(height) / float(width);
	Image image(width, height);
	VCDR_Renderer renderer(4, 4);

	float terrainHeight = 4.5f;
	float terrainScale = 0.15f;

	vec3 flatColor = vec3(0.3f, 0.3f, 0.0f);
	vec3 slopeColor = vec3(0.18f, 0.11f, 0.10f)*0.75f;

	VCDR_FBMTerrain terrain(terrainScale, terrainHeight, 4, 0.5f, 2.0f, 0.1f, 0.01f);

	vec3 lakeColor = 3.0f*vec3(0.3f, 0.48f, 1.0f);
	VCDR_Lake lake(0.55f, lakeColor);

	VCDR_SceneInfo info;
	// the sun  for the scene
	info.sun.dir = normalize(vec3(-0.6f, 0.4f, 0.6f));
	info.sun.col = 1.0f*vec3(1.0f, 0.6f, 0.3f);
	info.sun.size = 32.0f;
	info.sun.glareCol = 0.3f*vec3(2.0f, 0.6f, 0.3f);
	info.sun.glareSize = 4.0f;
	// sky height
	info.skyHeight = 1000.0f;
	// fog color
	info.fogColor = vec3(0.4f, 0.6f, 1.15f);
	info.fogAttenuation = 0.00005f;
	// diffuse light color
	info.lightColor = 6.0f*vec3(1.0f, 0.9f, 0.7f);

	// I picked the sky colors by hand, they're not derived in a special manner
	vec3 horizonColor = vec3(0.0f, 0.225f, 0.5f);
	vec3 zenithColor = vec3(0.76f, 0.945f, 1.0f);
	// get some fake sky
	VCDR_BackgroundSky sky(horizonColor, zenithColor, info.skyHeight);

	VCDR_World world(info, &sky);
	world.add(&terrain);
	world.add(&lake);

	vec3 trans = vec3(0.0f, -1.0f, 0.0f);
	vec3 cameraOrigin = vec3(0.0f, 5.0f, -6.0f) + trans;
	VCDR_CameraPerspective cam(cameraOrigin, vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), aspectRatio, 2.0f * rt::pi / 3.0f);

	vec3 cloudsScale = 0.9f*vec3(0.2f*4.0f, 0.2f*4.0f, 0.3f);
	float cloudsDensity = 0.04f;
	VCDR_Cloud cloud(vec3(0.0f, 8.0f, 6.0f), 5.0f*vec3(1.2f, 1.0f, 1.0f));
	VCDR_IntegratorDefault integrator(&cloud);

	renderer.render(image, &world, &cam, &integrator);
	image.writePNG("rendering_comp.png");
}


void rendering_competition()
{
	rs005();
}