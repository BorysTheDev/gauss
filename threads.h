#include "synchronization.h"
#include <string>
#include <cstdlib>
using namespace std;

char strj[10];
template<class Matrix>
class MultiThreadsGaussScheme
{
public:

  void start(int nThreads);
  MultiThreadsGaussScheme(Matrix &lu, Synchronization &s, int size,
      const int bs) :
        bn((size + (bs - 1)) / bs), bs(bs),  sync(s), luMatrix(lu), size(size)
  {
  }

private:
  const int bn;
  const int bs;
  Synchronization &sync;
  Matrix &luMatrix;
  int size;
  // algorithm
  class taskU;
  class taskL;
  int blockLU(Matrix &luMatrix, int size, const int bs);
};

template<class Matrix>
void MultiThreadsGaussScheme<Matrix>::start(int nThreads)
{
  taskU tu(luMatrix, &sync);
  taskL tl(luMatrix, &sync);
  for (int i = 0; i < nThreads; i++) {
    thread U(tu, size, bs);
    thread L(tl, size, bs);
  }
  blockLU(luMatrix, size, bs);
}

template<class Matrix>
class MultiThreadsGaussScheme<Matrix>::taskU
{
public:
  int operator()(int size, const int bs)
  {
    const int bn = (size + (bs - 1)) / bs;
    std::function<int(int)> lBorder = [&](int blockn) {
      return blockn * bs;
    };
    std::function<int(int)> rBorder = [&](int blockn) {
      return (blockn + 1) * bs > size ? size : (blockn + 1) * bs;
    };
    for (int j = sync->getUnprocessedColumnU(); j < bn;
        j = sync->getUnprocessedColumnU()) {
      //itoa(j,strj,10);
      for (int i = 0; i < j; i++) {
        sync->lockU(i, j);
        //sync.debugOut(string("U") + itoa(j,strj,10) + " i=" + itoa(i,strj,10));
        for (int k = 0; k < i; k++)
          for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
            for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
              for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
                luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
        for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
          for (int ik = lBorder(i); ik < ii; ik++)
            for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
              luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
        sync->unLockU(i);
      }
      sync->setProcessedColumnU(j);
    }
    return 0;
  }
  taskU(Matrix &lu, Synchronization * s): luMatrix(lu) { sync = s; }
private:
  Synchronization *sync;
  Matrix &luMatrix;
};

template<class Matrix>
class MultiThreadsGaussScheme<Matrix>::taskL
{
public:
  int operator()(int size, const int bs)
  {
    const int bn = (size + (bs - 1)) / bs;
    std::function<int(int)> lBorder = [&](int blockn) {
      return blockn * bs;
    };
    std::function<int(int)> rBorder = [&](int blockn) {
      return (blockn + 1) * bs > size ? size : (blockn + 1) * bs;
    };
    for (int j = sync->getUnprocessedRowL(); j < bn;
        j = sync->getUnprocessedRowL()) {
      for (int i = 0; i < j; i++) {
        sync->lockL(i, j);
        //sync.debugOut(string("L") + itoa(j,strj,10));
        for (int k = 0; k < i; k++)
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
              for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
                luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
        // L part last block
        for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++) {
          for (int ik = lBorder(i); ik < ii; ik++)
            for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
              luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            luMatrix[ij][ii] /= luMatrix[ii][ii];
        }
        sync->unLockL(i);
      }
      sync->setProcessedRowL(j);
    }
    return 0;
  }
  taskL(Matrix &lu, Synchronization * s) :
      luMatrix(lu)
  {
    sync = s;
  }
private:
  Synchronization *sync;
  Matrix &luMatrix;
};

template<class Matrix>
int MultiThreadsGaussScheme<Matrix>::blockLU(Matrix &luMatrix, int size, const int bs)
{
  std::function<int(int)> lBorder = [&](int blockn) {
    return blockn * bs;
  };
  std::function<int(int)> rBorder = [&](int blockn) {
    return (blockn + 1) * bs > size ? size : (blockn + 1) * bs;
  };
  for (int i = 0; i < bn; i++) {
    sync.lockMiddle(i);
    //sync.debugOut(string("M") + itoa(i,strj,10));
    cout << i << endl;
    // middle blocks j=i
    //  U part
    for (int k = 0; k < i; k++)
      for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
        for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
          for (int ij = lBorder(i); ij < rbi; ij++)
            luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
    // U and L part last block
    for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++) {
      for (int ik = lBorder(i); ik < ii; ik++) {
        for (int ij = ii; ij < rbi; ij++)
          luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
        for (int ij = ii + 1; ij < rbi; ij++)
          luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
      }
      //  end of L part
      for (int ij = ii + 1, rbi = rBorder(i); ij < rbi; ij++)
        luMatrix[ij][ii] /= luMatrix[ii][ii];
    }
    sync.unLockMiddle(i);
    sync.setProcessedMiddle(i);
  }
  return 0;
}
