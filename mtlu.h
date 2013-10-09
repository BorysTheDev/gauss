#ifndef MTLU_H_
#define MTLU_H_

template<class Matrix>
class MTBlockLU
{
public:

  void start(int nThreads);
  MTBlockLU(Matrix &lu,  int size, const int bs);

private:
  const int bs;
  const int bn;
  Matrix &luMatrix;
  int size;
  //blocks borders
  int lBorder(int blockn) {return blockn * bs;}
  int rBorder(int blockn) {return ++blockn * bs > size ? size : blockn * bs;}
  // algorithm
  void taskU(int j);
  void taskL(int j);
  void taskM(int j);
  // synchronization
};

template<class Matrix>
void MTBlockLU<Matrix>::start(int nThreads)
{
  for (int j = 0; j < bn; j++) {
    taskU(j);
    taskL(j);
    taskM(j);
  }
}

template<class Matrix>
MTBlockLU<Matrix>::MTBlockLU(Matrix &lu, int size, const int bs) :
    bs(bs), bn((size + (bs - 1)) / bs), luMatrix(lu), size(size)
{
}

template<class Matrix>
void MTBlockLU<Matrix>::taskU(int j)
{
  for (int i = 0; i < j; i++) {
    for (int k = 0; k < i; k++)
      for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
        for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
    // U part last block
    for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
      for (int ik = lBorder(i); ik < ii; ik++)
        for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
          luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
  }
}

template<class Matrix>
void MTBlockLU<Matrix>::taskL(int j)
{
  for (int i = 0; i < j; i++) {
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
  }
}

template<class Matrix>
void MTBlockLU<Matrix>::taskM(int j)
{
  //  full U and L part
  for (int k = 0; k < j; k++)
    for (int ii = lBorder(j), rbi = rBorder(j); ii < rbi; ii++)
      for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++)
        for (int ij = lBorder(j); ij < rbi; ij++)
          luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
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
}

// faster but not determined


#endif /* MTLU_H_ */
