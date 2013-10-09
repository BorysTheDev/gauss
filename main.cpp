#include <iostream>
#include "matrix_generator.h"
#include <time.h>  
#include "matrix.h"

#include "io.h"
#include "gauss.h"




int main(){

	int matrixSize = 2000;
	double** luMatrix = new double*[matrixSize];
	for (int i = 0; i < matrixSize; i++)
	  luMatrix[i] = new double[matrixSize];
	generateMatrix(luMatrix, matrixSize);

	double* x = new double[matrixSize];
	generateX(x, matrixSize);
	double* f = createEquationValue(luMatrix, x, matrixSize);

	float fTimeStart;
	fTimeStart = clock()/(float)CLOCKS_PER_SEC; 

	gaussMTBlockScheme(luMatrix, f, matrixSize, 2, 30);
	/*cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;
  fTimeStart = clock()/(float)CLOCKS_PER_SEC;
	blockLU(luMatrix1, matrixSize,100);
	reverseStroke(luMatrix, f, matrixSize);*/

	auto x2 = f;
	cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;


	writeVector("./gauss/result.txt", x2, matrixSize);
	//writeMatrix("./gauss/test.txt", luMatrix.data(),matrixSize);
	//writeMatrix("./gauss/test1.txt", luMatrix1.data(),matrixSize);


	delete[] x;
	delete[] f;
	cout << "end";
	return 0;
}
