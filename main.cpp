#include <iostream>
#include "matrix_generator.h"
#include <time.h>  
#include "matrix.h"

#include "io.h"
#include "gauss.h"
#include <thread>
#include "threads.h"
#include "mtlu.h"



int main(){

	int matrixSize = 2011;
	Matrix<double> luMatrix(matrixSize);
	generateMatrix(luMatrix);
	Matrix<double> luMatrix1(luMatrix);

	double* x = new double[matrixSize];
	generateX(x, matrixSize);
	double* f = createEquationValue(luMatrix, x, matrixSize);

	float fTimeStart;
	fTimeStart = clock()/(float)CLOCKS_PER_SEC; 
    //blockLU(luMatrix, 100);
	/*std::function<void(Matrix<double>,double*, const int size,
	  const int blockSize)> fun =
		[](Matrix<double> mx,double* f, const int size, const int blockSize){
		gaussBlockScheme(mx, f, size, blockSize);
	};
	std::thread t(fun,luMatrix, f, matrixSize, 100);
	t.join();*/

	//Synchronization s;
	//MultiThreadsGaussScheme<Matrix<double> > mtg(luMatrix, s, matrixSize, 100);
	//mtg.start(2);
	MTBlockLU<Matrix<double> > ntb(luMatrix, matrixSize, 100);
	ntb.start(2);
	reverseStroke(luMatrix, f, matrixSize);
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
