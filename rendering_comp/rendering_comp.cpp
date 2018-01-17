#include <rendering_comp/vcdr_renderer.h>
#include <core/image.h>

using namespace rt;

void rendering_competition()
{
	Image image(320, 240);
	VCDR_Renderer renderer;
	renderer.render(image);
	image.writePNG("rendering_comp.png");
}