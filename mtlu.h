#ifndef MTLU_H_
#define MTLU_H_
#include "synchronization.h"
#include <thread>
#include <list>
#include "immintrin.h"
// faster but not determined
template<class Matrix>
class MTBlockLU
{
public:

  void start(int nThreads);
  MTBlockLU(Matrix &lu, int size, const int bs);

private:
  const int bs;
  const int bn;
  Matrix &luMatrix;
  int size;
  Synchronization sync;
  //blocks borders
  int lBorder(int blockn)
  {
    return blockn * bs;
  }
  int rBorder(int blockn)
  {
    return ++blockn * bs > size ? size : blockn * bs;
  }

  typedef std::complex<double> cmpx;
  void mulAndSub(cmpx* to, cmpx* from1, cmpx* from2)
  {
      __m128d* v1 = (__m128d*)from1;
      double* v2 = (double*)from2;
      double* v3 = (double*)to;
      __m256d ymm0_v1;
      __m256d ymm1_v2;
      __m256d ymm2_axa;
      __m256d ymm3_perm_v2;
      __m256d ymm4_axb;
      __m256d ymm5_bb_xchange_ab;
      __m256d ymm6_aa_xchange_ab;
      __m256d ymm7_res;
      ymm0_v1 = _mm256_broadcast_pd(v1);
      ymm1_v2 = _mm256_loadu_pd(v2);
      //mul
      ymm2_axa = _mm256_mul_pd(ymm0_v1, ymm1_v2);
      ymm3_perm_v2 = _mm256_permute_pd (ymm1_v2, 0b0101);
      ymm4_axb = _mm256_mul_pd(ymm0_v1, ymm3_perm_v2);
      ymm5_bb_xchange_ab = _mm256_unpackhi_pd(ymm2_axa, ymm4_axb);
      ymm6_aa_xchange_ab = _mm256_unpacklo_pd(ymm2_axa, ymm4_axb);
      ymm7_res = _mm256_addsub_pd(ymm6_aa_xchange_ab, ymm5_bb_xchange_ab);
      //sub
      __m256d ymm9_v3 = _mm256_loadu_pd(v3);
      ymm9_v3 = _mm256_sub_pd(ymm9_v3, ymm7_res);
      _mm256_storeu_pd((double*)to, ymm9_v3);
  }

  // algorithm
  void taskU();
  void taskL();
  void taskM();
};

template<class Matrix>
void MTBlockLU<Matrix>::start(int nThreads)
{
  std::list<std::thread> threads;
  for(int i = 0; i < nThreads; i++) {
    threads.push_back(std::thread(&MTBlockLU<Matrix>::taskU, this));
    threads.push_back(std::thread(&MTBlockLU<Matrix>::taskL, this));
  }
    taskM();
  for (auto& t : threads) {
    t.join();
  }
}

template<class Matrix>
MTBlockLU<Matrix>::MTBlockLU(Matrix &lu, int size, const int bs) :
    bs(bs), bn((size + (bs - 1)) / bs), luMatrix(lu), size(size)
{
}

template<class Matrix>
void MTBlockLU<Matrix>::taskU()
{
  for (int j = sync.getUnprocessedColumnU(); j < bn;
      j = sync.getUnprocessedColumnU()) {
    for (int i = 0; i < j; i++) {
      sync.lockU(i, j);
      for (int k = 0; k < i; k++)
        for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
          for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
            for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij+=2)
              mulAndSub(&luMatrix[ii][ij], &luMatrix[ii][ik], &luMatrix[ik][ij]);
      // U part last block
      for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
        for (int ik = lBorder(i); ik < ii; ik++)
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
      sync.unLockU(i);
    }
    sync.setProcessedColumnU(j);
  }
}

template<class Matrix>
void MTBlockLU<Matrix>::taskL()
{
  for (int j = sync.getUnprocessedRowL(); j < bn; j =
      sync.getUnprocessedRowL()) {
    for (int i = 0; i < j; i++) {
      sync.lockL(i, j);
      for (int k = 0; k < i; k++)
        for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
          for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
            for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii+=2)
              mulAndSub(&luMatrix[ij][ii], &luMatrix[ij][ik], &luMatrix[ik][ii]);
      // L part last block
      for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++) {
        for (int ik = lBorder(i); ik < ii; ik++)
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
        for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
          luMatrix[ij][ii] /= luMatrix[ii][ii];
      }
      sync.unLockL(i);
    }
    sync.setProcessedRowL(j);
  }
}

template<class Matrix>
void MTBlockLU<Matrix>::taskM()
{
  for (int j = 0; j < bn; j++) {
    sync.lockMiddle(j);
    //  full U and L part
    for (int k = 0; k < j; k++)
      for (int ii = lBorder(j), rbi = rBorder(j); ii < rbi; ii++)
        for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
          for (int ij = lBorder(j); ij < rbi; ij+=2)
            mulAndSub(&luMatrix[ii][ij], &luMatrix[ii][ik], &luMatrix[ik][ij]);
    // U and L part last block
    for (int ii = lBorder(j), rbi = rBorder(j); ii < rbi; ii++) {
      for (int ik = lBorder(j); ik < ii; ik++) {
        for (int ij = ii; ij < rbi; ij++)
          luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
        for (int ij = ii + 1; ij < rbi; ij++)
          luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
      }
      //  end of L part
      for (int ij = ii + 1, rbi = rBorder(j); ij < rbi; ij++)
        luMatrix[ij][ii] /= luMatrix[ii][ii];
    }
    sync.unLockMiddle(j);
    sync.setProcessedMiddle(j);
  }
}

#endif /* MTLU_H_ */
