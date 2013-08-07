#ifndef LOCAL_SYNCHRONIZATION
#define LOCAL_SYNCHRONIZATION
#include "synchronization.h"
#include <vector> 
/*
template <class T>

class LocalSynchronization: public Synchronization {
public:
	// synchronization lock
	bool lockL(int n);
	bool lockU(int n);
	bool lockMiddle(int n);
	// synchronization unLock
	void unLockL(int n, int pos);
	void unLockU(int n, int pos);
	void unLockMiddle(int n){}
	// contructor
	LocalSynchronization(UnloadedBlockMatrix<T> &lu);
	~LocalSynchronization();
private:
	UnloadedBlockMatrix<T> &luMatrix;
	volatile int *usingBlocksL;
	volatile int *usingBlocksU;
	const int size;
};

template <class T>
LocalSynchronization<T>::LocalSynchronization(UnloadedBlockMatrix<T> &lu):luMatrix(lu), size(lu.getHeight()){
	usingBlocksL = new int[size];
	usingBlocksU = new int[size];
}

template <class T>
LocalSynchronization<T>::~LocalSynchronization(){
	delete[] usingBlocksL;
	delete[] usingBlocksU;
}

template <class T>
bool LocalSynchronization<T>::lockL(int n, int pos){
	lock lk(synchLUMonitor);
	while (n > processedMiddle)
		ReadyMiddle.wait(lk);
	if (luMatrix(j, n).getHeight() <= 0)
		luMatrix.loadBlock(j, n);
	if (usingBlocksU[n] == 0){
		for(int i = 0; i < n; i++)
			if (luMatrix(i, n).getHeight() <= 0)
				luMatrix.loadBlock(i, n);
	}
	usingBlocksU[n]++;
	return true;
}

template <class T>
bool LocalSynchronization<T>::lockU(int n, int pos){
	lock lk(synchLUMonitor);
	while (n > processedMiddle)
		ReadyMiddle.wait(lk);
	if (luMatrix(n, j).getHeight() <= 0)
		luMatrix.loadBlock(n, j);
	if (usingBlocksL[n] == 0){
		for(int i = 0; i < n; i++)
			if (luMatrix(n, i).getHeight() <= 0)
				luMatrix.loadBlock(n, i);
	}
	usingBlocksU[n]++;
	return true;
}

template <class T>
void LocalSynchronization<T>::unLockL(int n, int pos){
	lock lk(synchLUMonitor);
	if (usingBlocksU[n] == 0){
		for(int i = 0; i < n; i++)
			if (luMatrix(i, n).getHeight() <= 0)
				luMatrix.loadBlock(i, n);
	}
	usingBlocksU[n]++;
}

template <class T>
void LocalSynchronization<T>::unLockU(int n, int pos);{
}
*/
#endif /*LOCAL_SYNCHRONIZATION*/
