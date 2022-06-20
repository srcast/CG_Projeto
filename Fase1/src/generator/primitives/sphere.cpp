#include "generator/primitives/sphere.hpp"

using namespace brief_int;

auto generate_sphere(
    float radius,
    u32 num_slices,
    u32 num_stacks,
    fmt::ostream& output_file
) -> void {

    float rotationSlice = 2 * M_PI / num_slices; // 360 / nº slices
	float rotationStack = M_PI / num_stacks;     // 180 / nº stacks

	// x= r * cos(beta)  * sin(alpha);
	// y= r * sin(beta);
	// z= r * cos(beta)  * cos(alpha);

    int num_vertices = 2*3*num_slices + 2*3*(num_stacks-2)*num_slices; // (faces 1º e 2º stack) + (faces das restantes stacks)

    output_file.print("{}\n",num_vertices);

	for (int i = 0; i < num_stacks; i++) {

		float stackAngle = (M_PI / 2) - i * rotationStack; // beta
		float stackAngle2 = (M_PI / 2) - (i+1) * rotationStack; // beta da stack baixo

		for (int j = 0; j < num_slices; j++) {

			float sliceAngle = j * rotationSlice; // alpha
			float sliceAngle2 = (j+1) * rotationSlice; // alpha da slice lado

			if (i < num_stacks-1) { // todas as stacks menos a ultima de baixo
            	output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
            	output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle));
            	output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
			}
			else{ // stack final separada porque está ao contrario
				output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle2), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
			}

			if (i > 0 && i < num_stacks-1) {  // só as stacks que têm 2 triangulos por face
                output_file.print("{} {} {}\n",radius * cos(stackAngle2) * sin(sliceAngle2), radius * sin(stackAngle2), radius * cos(stackAngle2) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle2), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle2));
                output_file.print("{} {} {}\n",radius * cos(stackAngle) * sin(sliceAngle), radius * sin(stackAngle), radius * cos(stackAngle) * cos(sliceAngle));
			}
		}
	}
}
