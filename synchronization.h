#ifndef SYNCHRONIZATION
#define SYNCHRONIZATION
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
using namespace std;

class Synchronization{
public:
	// unprocessed elements are got
	virtual int getUnprocessedColumnU();
	virtual int getUnprocessedRowL();
	// synchronization lock
	virtual bool lockL(int n, int pos); // n is required middle element
	virtual bool lockU(int n, int pos); // n is required middle element
	virtual bool lockMiddle(int n);		// n is required L and U elements
	// synchronization unLock
	virtual void unLockL(int n){}
	virtual void unLockU(int n){}
	virtual void unLockMiddle(int n){}
	// processed elements
	virtual int setProcessedColumnU(int n);
	virtual int setProcessedRowL(int n);
	virtual int setProcessedMiddle(int n);
	//costructor
	Synchronization();
	// debug function
	void debugOut(string str){ lock lk(mutexio); cout << str << endl;}

protected:
	typedef unique_lock<std::mutex> lock;

	volatile int unprocessedColumnU;
	volatile int unprocessedRowL;
	volatile int processedColumnU;
	volatile int processedRowL;
	volatile int processedMiddle;
	// mutexs
	mutex synchMiddleMonitor;
	mutex synchLUMonitor;
	mutex mutexL;
	mutex mutexU;
	// debug mutex
	mutex mutexio;
	// conditions
	condition_variable ReadyRowL;
	condition_variable ReadyCollumnU;
	condition_variable ReadyMiddle;
};

Synchronization::Synchronization(){
	unprocessedColumnU = 1;
	unprocessedRowL = 1;
	// U and L haven't 0 block
	processedColumnU = 0;
	processedRowL = 0;
	processedMiddle = -1;
}

bool Synchronization::lockL(int n, int pos){
	if(n > processedMiddle){
		lock lk(synchLUMonitor);
		while (n > processedMiddle)
			ReadyMiddle.wait(lk);
	}
	return true;
}

bool Synchronization::lockU(int n, int pos){
	if(n > processedMiddle){
		lock lk(synchLUMonitor);
		while (n > processedMiddle)
			ReadyMiddle.wait(lk);
	}
	return true;
}

bool Synchronization::lockMiddle(int n){
	if(n > processedColumnU){
		lock lk(synchMiddleMonitor);
		while (n > processedColumnU)
			ReadyCollumnU.wait(lk);
	}
	if(n > processedRowL){
		lock lk(synchMiddleMonitor);
		while (n > processedRowL)
			ReadyRowL.wait(lk);
	}
	return true;
}

int Synchronization::getUnprocessedColumnU(){
	lock scoped_lock(mutexU);
	int temp = unprocessedColumnU;
	unprocessedColumnU++;
	return temp;
}

int Synchronization::getUnprocessedRowL(){
	lock scoped_lock(mutexL);
	int temp = unprocessedRowL;
	unprocessedRowL++;
	return temp;
}

int Synchronization::setProcessedColumnU(int n){
	lock lk(synchMiddleMonitor);
	int temp = processedColumnU;
	processedColumnU = n;
	ReadyCollumnU.notify_all();
	return temp;
}
int Synchronization::setProcessedRowL(int n){
	lock lk(synchMiddleMonitor);
	int temp = processedRowL;
	processedRowL = n;
	ReadyRowL.notify_all();
	return temp;
}
int Synchronization::setProcessedMiddle(int n){
	lock lk(synchLUMonitor);
	int temp = processedMiddle;
	processedMiddle = n;
	ReadyMiddle.notify_all();
	return temp;
}

#endif /*SYNCHRONIZATION*/
