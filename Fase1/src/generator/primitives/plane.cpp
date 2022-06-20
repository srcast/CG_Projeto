#include "generator/primitives/plane.hpp"

using namespace brief_int;

auto generate_plane(
    u32 comp,
    u32 div,
    fmt::ostream& output_file
) -> void {
    //static_cast<void>(len);
    //static_cast<void>(num_divs);
    //static_cast<void>(output_file);
	int num_vertices;

	num_vertices = 6 * (div * div);


	output_file.print("{}\n", num_vertices);

	float xNeg = -((float)comp / 2);
	//	float xPos = comp / 2;
	float zNeg = -((float)comp / 2);
	//	float zPos = comp / 2;

	float passoX = (float)comp / (float)div;
	float passoZ = (float)comp / (float)div;


	// divide por colunas
	for (int i = 1; i <= (int) div; i++) {
		// divide por linhas
		float zNeg = -((float) comp / 2.0);
		for (int d = 1; d <= (int) div; d++) {

			// PONTOS DO PRIMIERO TRIANGULO ----- > p3 tem de vir antes do p2 devido à regra da mão direita
			// p1 = (xNeg, 0, zNeg);
			output_file.print("{} 0.0 {}\n", xNeg, zNeg);
			// p3 = (xNeg, 0, zNeg + passoZ);
			output_file.print("{} 0.0 {}\n", xNeg, zNeg + passoZ);
			// p2 = (xNeg + passoX, 0, zNeg);
			output_file.print("{} 0.0 {}\n", xNeg + passoX, zNeg);


			// PONTOS DO SEGUNDO TRIANGULO
			// p2 = (xNeg + passoX, 0, zNeg);
			output_file.print("{} 0.0 {}\n", xNeg + passoX, zNeg);
			// p3 = (xNeg, 0, zNeg + passoZ);
			output_file.print("{} 0.0 {}\n", xNeg, zNeg + passoZ);
			// p4 = (xNeg + passoX, 0, zNeg + passoZ);
			output_file.print("{} 0.0 {}\n", xNeg + passoX, zNeg + passoZ);

			zNeg += passoZ;
		}

		xNeg += passoX;
		//printf("%f %f\n",xNeg,zNeg);
	}

	//plane.close();
}
