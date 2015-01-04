#include <iostream>
#include "matrix_generator.h"
#include <time.h>  

#include "io.h"
#include "gauss.h"




int main(){

    int matrixSize = 4000;
	double** luMatrix = new double*[matrixSize];
	for (int i = 0; i < matrixSize; i++)
	  luMatrix[i] = new double[matrixSize];
	generateMatrix(luMatrix, matrixSize);

	double* x = new double[matrixSize];
	generateX(x, matrixSize);
	double* f = createEquationValue(luMatrix, x, matrixSize);

    Timer timer;
    timer.start();

    gaussMTBlockScheme(luMatrix, f, matrixSize, 4, 30);
	/*cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;
  fTimeStart = clock()/(float)CLOCKS_PER_SEC;
  gaussScheme(luMatrix, f, matrixSize);*/
    timer.stop();
	auto x2 = f;
    cout <<timer.interval()<<endl;


	writeVector("./gauss/result.txt", x2, matrixSize);
	//writeMatrix("./gauss/test.txt", luMatrix.data(),matrixSize);
	//writeMatrix("./gauss/test1.txt", luMatrix1.data(),matrixSize);


	delete[] x;
	delete[] f;
	cout << "end";
	return 0;
}
