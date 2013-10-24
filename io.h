#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

void writeVector(string fileName, double* vector, int size) {
	ofstream fout(fileName);
	//fout.setf(ios_base::fixed,ios_base::floatfield);
	fout <<size<< endl;
	for (int i = 0; i < size; i++) {
		fout << vector[i]<<endl;
	}
	fout.flush();
	fout.close();
}

template <class T>
void writeMatrix(string fileName, T matrix, int size) {
	ofstream fout(fileName);
	fout <<size<< endl;
	fout.setf(ios_base::scientific);
	for (int i = 0; i <size; i++) {
		for (int j = 0; j < size; j++)
			fout << matrix[i][j] << " ";
		fout << endl;
	}
	fout.flush();
	fout.close();
}
