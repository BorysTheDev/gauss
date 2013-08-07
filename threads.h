
#include "synchronization.h"
#include <string>
#include <cstdlib>
using namespace std;
/*
char strj[10];
template <class T>
class MultiThreadsGaussScheme{
public:
	typedef Block<Block<T>> TBlockMatrix;

	void start(int nThreads);
	MultiThreadsGaussScheme(TBlockMatrix &lu, Synchronization &s):luMatrix(lu), sync(s){}

private:

	Synchronization &sync;
	TBlockMatrix &luMatrix;
	// algorithm
	class taskU;
	class taskL;
	int  blockLU(TBlockMatrix &luMatrix);
};

template <class T>
void MultiThreadsGaussScheme<T>::start(int nThreads){
	taskU tu(luMatrix, &sync);
	taskL tl(luMatrix, &sync);
	for (int i = 0; i < nThreads; i++){
		thread U(tu);
		thread L(tl);
	}
	blockLU(luMatrix);
}

template <class T>
class MultiThreadsGaussScheme<T>::taskU{
public:
	int operator()(){
		for (int j = sync->getUnprocessedColumnU(); j < luMatrix.getWidth(); j = sync->getUnprocessedColumnU()){
			//itoa(j,strj,10);
			for (int i = 0; i < j; i++){
				sync->lockU(i, j);
				//sync.debugOut(string("U") + itoa(j,strj,10) + " i=" + itoa(i,strj,10));
				for (int k = 0; k < i; k++){
					for(int ii = 0; ii < luMatrix(i, j).getHeight(); ii++)
						for(int ik = 0; ik < luMatrix(i, j).getHeight(); ik++)
							for(int ij = 0; ij < luMatrix(i, j).getWidth(); ij++)
								luMatrix(i, j)(ii, ij) -= luMatrix(i, k)(ii, ik) * luMatrix(k, j)(ik, ij);
				}
				for(int ii = 0; ii < luMatrix(i, 0).getHeight(); ii++)
					for (int ik = 0; ik < ii; ik++)
						for (int ij = 0; ij < luMatrix(i, j).getWidth(); ij++)
							luMatrix(i, j)(ii, ij) -= luMatrix(i, i)(ii, ik) * luMatrix(i, j)(ik, ij);
				sync->unLockU(i);
			}
			sync->setProcessedColumnU(j);
		}
		return 0;
	}
	taskU(TBlockMatrix &lu, Synchronization * s):luMatrix(lu){
		sync = s;
	}
private:
	Synchronization *sync;
	TBlockMatrix &luMatrix;
};

template <class T>
class MultiThreadsGaussScheme<T>::taskL{
public:
	int operator()(){
		for (int j = sync->getUnprocessedRowL(); j < luMatrix.getHeight(); j = sync->getUnprocessedRowL()){
			for (int i = 0; i < j; i++){
				sync->lockL(i, j);
				//sync.debugOut(string("L") + itoa(j,strj,10));
				for (int k = 0; k < i; k++){
					for (int ii = 0; ii < luMatrix(i, 0).getHeight(); ii++)
						for (int ik = 0; ik < luMatrix(i, 0).getHeight(); ik++)
							for (int ij = 0; ij < luMatrix(j, 0).getHeight(); ij++)
								luMatrix(j, i)(ij, ii) -= luMatrix(j, k)(ij, ik) * luMatrix(k, i)(ik, ii);
				}
				for (int ii = 0; ii < luMatrix(i, 0).getHeight(); ii++){
					for (int ik = 0; ik < ii; ik++)
						for (int ij = 0; ij < luMatrix(i, j).getWidth(); ij++)
							luMatrix(j, i)(ij, ii) -= luMatrix(j, i)(ij, ik) * luMatrix(i, i)(ik, ii);
					for (int ij = 0; ij < luMatrix(i, j).getWidth(); ij++)
						luMatrix(j, i)(ij, ii) /= luMatrix(i, i)(ii, ii);
				}
				sync->unLockL(i);
			}
			sync->setProcessedRowL(j);
		}
		return 0;
	}
	taskL(TBlockMatrix &lu, Synchronization * s):luMatrix(lu){
		sync = s;
	}
private:
	Synchronization *sync;
	TBlockMatrix &luMatrix;
};

template <class T>
int MultiThreadsGaussScheme<T>::blockLU(TBlockMatrix &luMatrix){
	for (int i = 0; i < luMatrix.getHeight(); i++) {
		sync.lockMiddle(i);
		//sync.debugOut(string("M") + itoa(i,strj,10));
		cout << i << endl;
		// middle blocks j=i
		//  U part
		for (int k = 0; k < i; k++){
			for (int ii = 0; ii < luMatrix(i, k).getHeight(); ii++) {
				for (int ik = 0; ik < luMatrix(k, i).getHeight(); ik++)
					for (int ij = 0; ij < luMatrix(k, i).getWidth(); ij++)
						luMatrix(i, i)(ii, ij) -= luMatrix(i, k)(ii, ik) * luMatrix(k, i)(ik, ij);
			}
		}
		//  L part
		for (int ii = 0; ii < luMatrix(i, i).getHeight(); ii++) {
			for (int ik = 0; ik < ii; ik++)
				for (int ij = ii; ij < luMatrix(i, i).getWidth(); ij++)
					luMatrix(i, i)(ii, ij) -= luMatrix(i, i)(ii, ik) * luMatrix(i, i)(ik, ij);
			for (int ik = 0; ik < ii; ik++)
				for (int ij = ii + 1; ij < luMatrix(i, i).getWidth(); ij++)
					luMatrix(i, i)(ij, ii) -= luMatrix(i, i)(ij, ik) * luMatrix(i, i)(ik, ii);
			for (int ij = ii + 1; ij < luMatrix(i, i).getWidth(); ij++)
				luMatrix(i, i)(ij, ii) /= luMatrix(i, i)(ii, ii);
		}
		sync.unLockMiddle(i);
		sync.setProcessedMiddle(i);
	}
	return 0;
}*/
