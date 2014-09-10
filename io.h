#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>
#include <string>
#include <utility>
#include <list>
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

class Timer{
public:
    typedef std::chrono::time_point<std::chrono::system_clock> timePoint;
    void start();
    void stop();
    double interval();
private:
    timePoint start_, stop_;
};

void Timer::start(){
    start_ = std::chrono::system_clock::now();
}

void Timer::stop(){
    stop_ = std::chrono::system_clock::now();
}

double Timer::interval(){
     return ((double)std::chrono::duration_cast<std::chrono::milliseconds>(stop_-start_).count()) / 1000;
}
