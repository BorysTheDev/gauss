#include <iostream>
#include "matrix_generator.h"
#include <time.h>

#include "io.h"
#include "gauss.h"

using namespace std;

double AvxVecMul(double* v1, double* v2, int size)
{
  __m256d ymm0;
  __m256d ymm1;
  __m256d ymm2;
  __m256d ymm3 = _mm256_setzero_pd();
  for(int i = 0; i < size; i += 4)
  {
     ymm0 = _mm256_loadu_pd(v1);
     ymm1 = _mm256_loadu_pd(v2);
     ymm2 = _mm256_mul_pd(ymm0, ymm1);
     ymm3 = _mm256_add_pd(ymm3, ymm2);
     v1 += 4;
     v2 += 4;
  }
  alignas(32) double res[4];
  _mm256_stream_pd(res, ymm3);

  return res[0]+res[1]+res[2]+res[3];
}

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

    gaussBlockScheme(luMatrix, f, matrixSize, 80);
    cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;
    //fTimeStart = clock()/(float)CLOCKS_PER_SEC;
    //gaussScheme(luMatrix, f, matrixSize);

    //cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;


    writeVector("result.txt", f, matrixSize);
    //writeMatrix("./gauss/test.txt", luMatrix.data(),matrixSize);
    //writeMatrix("./gauss/test1.txt", luMatrix1.data(),matrixSize);


    delete[] x;
    delete[] f;
    cout << "end";
    return 0;
}
