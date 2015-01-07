#include <iostream>
#include "matrix_generator.h"
#include <time.h>

#include "io.h"
#include "gauss.h"
#include "immintrin.h"
using namespace std;

std::complex<double> AvxVecMul(std::complex<double>* v1_c, std::complex<double>* v2_c, int size)
{
    double* v1 = (double*)v1_c;
    double* v2 = (double*)v2_c;
  __m256d ymm0_v1;
  __m256d ymm1_v2;
  __m256d ymm2_axa;
  __m256d ymm3_perm_v2;
  __m256d ymm4_axb;
  __m256d ymm5_bb_xchange_ab;
  __m256d ymm6_aa_xchange_ab;
  __m256d ymm7_res;
  __m256d ymm8_sum = _mm256_setzero_pd();
  for(int i = 0; i < size; i += 2)
  {
     ymm0_v1 = _mm256_loadu_pd(v1);
     ymm1_v2 = _mm256_loadu_pd(v2);
     ymm2_axa = _mm256_mul_pd(ymm0_v1, ymm1_v2);
     ymm3_perm_v2 = _mm256_permute_pd (ymm1_v2, 0b0101);
     ymm4_axb = _mm256_mul_pd(ymm0_v1, ymm3_perm_v2);
     ymm5_bb_xchange_ab = _mm256_unpackhi_pd(ymm2_axa, ymm4_axb);
     ymm6_aa_xchange_ab = _mm256_unpacklo_pd(ymm2_axa, ymm4_axb);
     ymm7_res = _mm256_addsub_pd(ymm6_aa_xchange_ab, ymm5_bb_xchange_ab);
     ymm8_sum = _mm256_add_pd(ymm7_res, ymm8_sum);
     v1 += 4;
     v2 += 4;
  }
  alignas(32) std::complex<double> res[2];
  _mm256_stream_pd((double*)res, ymm8_sum);

  return res[0]+res[1];
}

std::complex<double> VecMul(std::complex<double>* v1, std::complex<double>* v2, int size)
{
    std::complex<double> sum = 0.0;
    for (int i = 0; i < size; i++)
        sum += v1[i] * v2[i];

  return sum;
}

//int main()
//{
//  double vec[4] = {1,2,3,4};
//  __m256d ymm0 = _mm256_loadu_pd(vec);
//  __m256d ymm1 = _mm256_permute_pd (ymm0, 0b0101);
//  _mm256_storeu_pd(vec, ymm1);
//  std::cout << vec[0] << std::endl
//    << vec[1] << std::endl
//    << vec[2] << std::endl
//    << vec[3] << std::endl;
//  return 1;
//}

//int main()
//{
//    const int SIZE = 10000000;
//    std::complex<double>* v1 = new std::complex<double>[SIZE];
//    std::complex<double>* v2 = new std::complex<double>[SIZE];

//    for (int i = 0; i < SIZE; i++)
//    {
//        v1[i] = std::complex<double>(i, i / 3);
//        v2[i] = v1[i] + i / 2.0;
//    }
//    Timer timer;
//    timer.start();
//    std::complex<double> test = VecMul(v1, v2, SIZE);
//    timer.stop();
//    cout << test << " " << timer.interval()<<endl;
//    timer.start();
//    std::complex<double> test1 = AvxVecMul(v1, v2, SIZE);
//    timer.stop();
//    cout << test1 << " " << timer.interval()<<endl;
//    return 0;
//}

int main(){

    int matrixSize = 1731;
    std::complex<double>** luMatrix = new  std::complex<double>*[matrixSize];
    for (int i = 0; i < matrixSize; i++)
      luMatrix[i] = new  std::complex<double>[matrixSize];
    generateMatrix(luMatrix, matrixSize);

    std::complex<double>* x = new std::complex<double>[matrixSize];
    generateX(x, matrixSize);
     std::complex<double>* f = createEquationValue(luMatrix, x, matrixSize);

    Timer timer;
    timer.start();

    gaussScheme(luMatrix, f, matrixSize);
    timer.stop();
    cout <<timer.interval()<<endl;
    //fTimeStart = clock()/(float)CLOCKS_PER_SEC;
    //gaussScheme(luMatrix, f, matrixSize);

    //cout <<clock()/(float)CLOCKS_PER_SEC - fTimeStart<<endl;


    writeVector("C:\\\\Users\\equal\\Desktop\\tournament-autoregistration\\result.txt", f, matrixSize);
    //writeMatrix("./gauss/test.txt", luMatrix.data(),matrixSize);
    //writeMatrix("./gauss/test1.txt", luMatrix1.data(),matrixSize);


    delete[] x;
    delete[] f;
    cout << "end";
    return 0;
}
