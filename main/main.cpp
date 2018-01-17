
void a_julia();
void a_cameras();
void a_solids();
void a_indexing();
void a_instancing();
void a_lighting();
void a_materials();
void a_distributed();
void a_textures();
void a_mappers();
void a_smooth();
void a_bumpmappers();
//void rendering_competition();
#include <core/matrix.h>
#include <iostream>
int main(int argc, char* argv[])
{
    a_smooth();
	a_bumpmappers();
	rt::Matrix s = rt::Matrix::system(rt::Vector(1.0f, 0.0f, 0.0f), rt::Vector(0.0f, 1.0f, 0.0f), rt::Vector(0.0f, 0.0f, 1.0f));
	std::cout << s;
	//rendering_competition();
    return 0;
}
