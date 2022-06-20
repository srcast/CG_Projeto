#include "generator/primitives/box.hpp"

using namespace brief_int;

auto generate_box(
    u32 comp,
    u32 div,
    fmt::ostream& output_file
) -> void {
    //static_cast<void>(num_units);
    //static_cast<void>(grid_len);
    //static_cast<void>(output_file);
	u32 num_vertices;

	num_vertices = 6 * 6 * (div * div);

	//std::ofstream box;

	//box.open("box.3d", std::ofstream::out | std::ios_base::app);

	output_file.print("{}\n", num_vertices);

	// lado esquerdo
	geraPlanoBox('x', (float) comp, (int) (int) div, -1, output_file);

	// lado direito
	geraPlanoBox('x', (float) comp, (int) div, 1, output_file);

	// lado frente
	geraPlanoBox('z', (float) comp, (int) div, -1, output_file);

	// lado trás
	geraPlanoBox('z', (float) comp, (int) div, 1, output_file);

	// base
	geraPlanoBox('y', (float) comp, (int) div, -1, output_file);

	// topo
	geraPlanoBox('y', (float) comp, (int) div, 1, output_file);


}


auto geraPlanoBox(
	char k,
    float comp,
    int div,
    int v,
	fmt::ostream& output_file
) -> void {

	//std::ofstream box;

	//box.open("box.3d", std::ofstream::out | std::ios_base::app); // append or write

	//ofstream box("box.3d");

	float xNeg = -(comp / 2);
	float yNeg = -(comp / 2);
	float zNeg = -(comp / 2);
	float xPos = comp / 2;
	float yPos = comp / 2;
	float zPos = comp / 2;

	float passoX = comp / div;
	float passoY = comp / div;
	float passoZ = comp / div;

	u32 i, d;

	if (k == 'x') { // lado esquerdo e direito ---> depende do v
		if (v == -1) {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					output_file.print("{} {} {}\n", xPos * v, yPos, zPos);
					// p2
					output_file.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);
					// p3
					output_file.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);



					// PONTOS DO SEGUNDO TRIANGULO
					// p3
					output_file.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);
					// p2
					output_file.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);
					// p4
					output_file.print("{} {} {}\n", xPos * v, yPos - passoY, zPos - passoZ);

					yPos -= passoY;
				}

				zPos -= passoZ;
			}
		}
		else {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					output_file.print("{} {} {}\n", xPos * v, yPos, zPos);
					// p3
					output_file.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);
					// p2
					output_file.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);




					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					output_file.print("{} {} {}\n", xPos * v, yPos, zPos - passoZ);
					// p3
					output_file.print("{} {} {}\n", xPos * v, yPos - passoY, zPos);
					// p4
					output_file.print("{} {} {}\n", xPos * v, yPos - passoY, zPos - passoZ);

					yPos -= passoY;
				}

				zPos -= passoZ;
			}
		}
	}
	else if (k == 'y') { // topo e base ----> depende do v
		if (v == -1) {
			// divide por colunas
			for (int i = 1; i <= div; i++) {
				// divide por linhas
				float zNeg = -(comp / 2);
				for (int d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO ----- > p3 tem de vir antes do p2 devido à regra da mão direita
					// p1
					output_file.print("{} {} {}\n", xNeg, yPos * v, zNeg);
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);


					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);
					// p4
					output_file.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg + passoZ);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);

					zNeg += passoZ;
				}

				xNeg += passoX;
			}
		}
		else {
			// divide por colunas
			for (int i = 1; i <= div; i++) {
				// divide por linhas
				float zNeg = -(comp / 2);
				for (int d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO ----- > p3 tem de vir antes do p2 devido à regra da mão direita
					// p1
					output_file.print("{} {} {}\n", xNeg, yPos * v, zNeg);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);


					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos * v, zNeg + passoZ);
					// p4
					output_file.print("{} {} {}\n", xNeg + passoX, yPos * v, zNeg + passoZ);

					zNeg += passoZ;
				}

				xNeg += passoX;
			}
		}
	}
	else if (k == 'z') { // frente e trás ----> depende do v
		if (v == -1) {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					output_file.print("{} {} {}\n", xNeg, yPos, zPos * v);
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);



					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);
					// p4
					output_file.print("{} {} {}\n", xNeg + passoX, yPos - passoY, zPos * v);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);

					yPos -= passoY;
				}

				xNeg += passoX;
			}
		}
		else {
			// divide por colunas
			for (i = 1; i <= div; i++) {
				// divide por linhas
				float yPos = comp / 2;
				for (d = 1; d <= div; d++) {

					// PONTOS DO PRIMIERO TRIANGULO
					// p1
					output_file.print("{} {} {}\n", xNeg, yPos, zPos * v);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);



					// PONTOS DO SEGUNDO TRIANGULO
					// p2
					output_file.print("{} {} {}\n", xNeg + passoX, yPos, zPos * v);
					// p3
					output_file.print("{} {} {}\n", xNeg, yPos - passoY, zPos * v);
					// p4
					output_file.print("{} {} {}\n", xNeg + passoX, yPos - passoY, zPos * v);

					yPos -= passoY;
				}

				xNeg += passoX;
			}
		}
	}
//	else {
//		cout << "Nao recebi um valor 0 no geraPlanoBox\n";
//	}

	//box.close();




}