#include <iostream>
#include "matrix_generator.h"
#include <time.h>  
#include "matrix.h"
#include "lu.h"
#include "io.h"

using namespace std;


int main(){

	int matrixSize = 2231;
	Matrix<double> luMatrix(matrixSize);
	generateMatrix(luMatrix);
	Matrix<double> luMatrix1(luMatrix);

	double* x = new double[matrixSize];
	generateX(x, matrixSize);
	double* f = createEquationValue(luMatrix, x, matrixSize);

	float fTimeStart;
	fTimeStart = clock()/(float)CLOCKS_PER_SEC; 
    //blockLU(luMatrix, 100);
    simpleLU(luMatrix);
	cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;

	double* x2 = reverseStroke<double>(luMatrix, f, matrixSize);
	writeVector("./gauss/result.txt", x2, matrixSize);
	//writeMatrix("./gauss_scheme/test.txt", luMatrix.data(),
	//    matrixSize);
	//writeMatrix("./gauss_scheme/test1.txt", luMatrix1.data(),
	//     matrixSize);


	delete[] x;
  delete[] x2;
	delete[] f;
	cout << "end";
	return 0;
}
