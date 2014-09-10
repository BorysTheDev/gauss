#include <cstdlib>
#include <complex>
using namespace std;

void generateMatrix(std::complex<double>** matrix, int size) {
    for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      matrix[i][j] = rand();
}

std::complex<double>* createEquationValue(std::complex<double>** matrix, std::complex<double>* x, int size) {
    std::complex<double>* f = new std::complex<double>[size];
    for (int i = 0; i < size; i++) {
        f[i] = 0;
        for (int j = 0; j < size; j++)
            f[i] += matrix[i][j] * x[j];
    }
    return f;
}


void generateX(std::complex<double>* x, int size) {
    for (int i = 0; i < size; i++)
    {
        x[i] = i + 1;
    }
}

