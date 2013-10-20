#ifndef SYNCHRONIZATION
#define SYNCHRONIZATION
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class Synchronization{
public:
	// unprocessed elements are got
	virtual int getUnprocessedColumnU() { return unprocessedColumnU++; }
	virtual int getUnprocessedRowL()    { return unprocessedRowL++; }
	// synchronization lock
	virtual bool lockL(int n, int pos); // n is required middle element
	virtual bool lockU(int n, int pos); // n is required middle element
	virtual bool lockMiddle(int n);		// n is required L and U elements
	// synchronization unLock
	virtual void unLockL(int n) {}
	virtual void unLockU(int n) {}
	virtual void unLockMiddle(int n) {}
	// processed elements
	virtual int setProcessedColumnU(int n);
	virtual int setProcessedRowL(int n);
	virtual int setProcessedMiddle(int n);
	//costructor
	Synchronization();
	// debug function
	//void debugOut(string str){ lock lk(mutexio); cout << str << endl;}

protected:
	typedef std::unique_lock<std::mutex> lock;

	std::atomic_int unprocessedColumnU;
	std::atomic_int unprocessedRowL;
	std::atomic_int processedColumnU;
	std::atomic_int processedRowL;
	std::atomic_int processedMiddle;
	// mutexs
	std::mutex synch;
	//std::mutex mutexio;
	// conditions
	std::condition_variable ReadyRowL;
	std::condition_variable ReadyCollumnU;
	std::condition_variable ReadyMiddle;
};

Synchronization::Synchronization(){
	unprocessedColumnU = 1;
	unprocessedRowL = 1;
	// U and L haven't 0 block
	processedColumnU = 0;
	processedRowL = 0;
	processedMiddle = -1;
}

bool Synchronization::lockL(int n, int pos)
{
  if (n > processedMiddle) {
    lock lk(synch);
    while (n > processedMiddle)
      ReadyMiddle.wait(lk);
  }
  return true;
}

bool Synchronization::lockU(int n, int pos){
  if (n > processedMiddle) {
    lock lk(synch);
    while (n > processedMiddle)
      ReadyMiddle.wait(lk);
  }
  return true;
}

bool Synchronization::lockMiddle(int n){
  if ((n > processedColumnU) || (n > processedRowL)) {
    lock lk(synch);
    while (n > processedColumnU)
      ReadyCollumnU.wait(lk);
    while (n > processedRowL)
      ReadyRowL.wait(lk);
  }
  return true;
}


int Synchronization::setProcessedColumnU(int n){
	int temp = processedColumnU.exchange(n);
	ReadyCollumnU.notify_all();
	return temp;
}
int Synchronization::setProcessedRowL(int n){
	int temp = processedRowL.exchange(n);
	ReadyRowL.notify_all();
	return temp;
}
int Synchronization::setProcessedMiddle(int n){
	int temp = processedMiddle.exchange(n);;
	ReadyMiddle.notify_all();
	return temp;
}

#endif /*SYNCHRONIZATION*/
