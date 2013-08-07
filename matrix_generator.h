#include <cstdlib>
#include "matrix.h"
using namespace std;

void generateMatrix(Matrix<double>& matrix) {
  for (size_t i = 0; i < matrix.width(); i++)
    for (size_t j = 0; j < matrix.height(); j++)
      matrix[i][j] = rand();
}

double* createEquationValue(Matrix<double>& matrix, double* x, int size) {
	double* f = new double[size];
	for (int i = 0; i < size; i++) {
		f[i] = 0;
		for (int j = 0; j < size; j++)
			f[i] += matrix[i][j] * x[j];
	}
	return f;
}

void generateX(double* x, int size) {
	for (int i = 0; i < size; i++)
		x[i] = i + 1;
}

