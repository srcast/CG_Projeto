#include "generator/primitives/cone.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace brief_int;

auto generate_cone(
    float radius,
    float height,
    u32 slices,
    u32 stacks,
    fmt::ostream& output_file
) -> void {
    //static_cast<void>(radius);
    //static_cast<void>(height);
    //static_cast<void>(num_slices);
    //static_cast<void>(num_stacks);
    //static_cast<void>(output_file);
    float step_lado = 2 * M_PI / (float)slices;
	float step_altura = height/stacks;
	float alpha = 0;
	float x, y, z;
	float rx1, rz1, rx2, rz2;
	float tmp1, tmp2;
    output_file.print("{}\n", ((stacks-1)*slices*2+slices*2)*3);

    //Desenha lado a lado a partir da base até ao topo do cone
	for (int i = 0; i < slices; i++) {
		//Base
		y = 0;
		x = radius * sin(i * step_lado);
		z = radius * cos(i * step_lado);
		rx1 = x;
		rz1 = z;
		//glVertex3f(x, y, z);
		//glVertex3f(0, 0, 0);
        output_file.print("{} {} {}\n", x, y, z);
        output_file.print("0 0 0\n");

		alpha += step_lado;
		x = radius * sin(alpha);
		z = radius * cos(alpha);
		rx2 = x;
		rz2 = z;
		//glVertex3f(x, y, z);
        output_file.print("{} {} {}\n", x, y, z);

		float antx = rx1, antz = rz1;
		float antx2 = rx2, antz2 = rz2;

		tmp2 = height;
		for (int j = 1; j < stacks; j++) {
			tmp1 = ((stacks - (float)j) / stacks);

			//glVertex3f(antx, y, antz);
			//glVertex3f(antx2, y, antz2);
			//glVertex3f(tmp1 * rx2, step_altura * j, tmp1 * rz2);
            output_file.print("{} {} {}\n", antx, y, antz);
            output_file.print("{} {} {}\n", antx2, y, antz2);
            output_file.print("{} {} {}\n", tmp1 * rx2, step_altura * j, tmp1 * rz2);


			//glVertex3f(antx, y, antz);
			//glVertex3f(tmp1 * rx2, step_altura * j, tmp1 * rz2);
			//glVertex3f(tmp1 * rx1, step_altura * j, tmp1 * rz1);
            output_file.print("{} {} {}\n", antx, y, antz);
            output_file.print("{} {} {}\n", tmp1 * rx2, step_altura * j, tmp1 * rz2);
            output_file.print("{} {} {}\n", tmp1 * rx1, step_altura * j, tmp1 * rz1);


			antx = tmp1 * rx1;
			antz = tmp1 * rz1;
			antx2 = tmp1 * rx2;
			antz2 = tmp1 * rz2;
			y = step_altura * j;
		}


		//glVertex3f(antx, y, antz);
		//glVertex3f(antx2, y, antz2);
		//glVertex3f(0, height, 0);
        output_file.print("{} {} {}\n", antx, y, antz);
        output_file.print("{} {} {}\n", antx2, y, antz2);
        output_file.print("{} {} {}\n", 0, height, 0);
	}
}
