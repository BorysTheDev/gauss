#include <iostream>
#include "matrix_generator.h"
#include <time.h>  
#include "matrix.h"

#include "io.h"
#include "gauss.h"

using namespace std;


int main(){

	int matrixSize = 1999;
	Matrix<double> luMatrix(matrixSize);
	generateMatrix(luMatrix);
	Matrix<double> luMatrix1(luMatrix);

	double* x = new double[matrixSize];
	generateX(x, matrixSize);
	double* f = createEquationValue(luMatrix, x, matrixSize);

	float fTimeStart;
	fTimeStart = clock()/(float)CLOCKS_PER_SEC; 
    //blockLU(luMatrix, 100);
	gaussBlockScheme(luMatrix, f, matrixSize, 100);
	auto x2 = f;
	cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;


	writeVector("./gauss/result.txt", x2, matrixSize);
	//writeMatrix("./gauss_scheme/test.txt", luMatrix.data(),
	//    matrixSize);
	//writeMatrix("./gauss_scheme/test1.txt", luMatrix1.data(),
	//     matrixSize);


	delete[] x;
	delete[] f;
	cout << "end";
	return 0;
}
