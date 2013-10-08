#include <functional>

//Matrix size must be >=1
template<class Matrix>
void simpleLU(Matrix& luMatrix, int size)
{
  for (size_t i = 0; i < size; i++) {
    for (size_t k = 0; k < i; k++)
      for (size_t j = i; j < size; j++)
        luMatrix(i, j) -= luMatrix(i, k) * luMatrix(k, j);

    for (size_t j = i + 1; j < size; j++) {
      decltype(luMatrix[0][0] + 0) sum = 0;
      for (size_t k = 0; k < i; k++)
        sum += luMatrix(j, k) * luMatrix(k, i);
      luMatrix(j, i) = (luMatrix(j, i) - sum) / luMatrix(i, i);
    }
  }
}

template<class Matrix>
void blockLU(Matrix& luMatrix, int size, const int bs)
{
  const int bn = (size + (bs - 1)) / bs;

  std::function<int(int)> lBorder = [&](int blockn) {
    return blockn * bs;
  };
  std::function<int(int)> rBorder = [&](int blockn) {
    return (blockn + 1) * bs > size ? size : (blockn + 1) * bs;
  };

  for (int i = 0; i < bn; i++) {
    // middle blocks
    //  full U and L part
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

    //U blocks
    for (int j = i + 1; j < bn; j++) {
      for (int k = 0; k < i; k++) {
        for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
          for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++) {
            for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
              luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
          }
      }
      // U part last block
      for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
        for (int ik = lBorder(i); ik < ii; ik++) {
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            luMatrix[ii][ij] -= luMatrix[ii][ik] * luMatrix[ik][ij];
        }
    }

    //L blocks
    for (int j = i + 1; j < bn; j++) {
      for (int k = 0; k < i; k++) {
        for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
          for (int ik = lBorder(k), rbk = rBorder(k); ik < rbk; ik++) {
            for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++)
              luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
          }
      }
      // L part last block
      for (int ii = lBorder(i), rbi = rBorder(i); ii < rbi; ii++) {
        for (int ik = lBorder(i); ik < ii; ik++) {
          for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
            luMatrix[ij][ii] -= luMatrix[ij][ik] * luMatrix[ik][ii];
        }
        for (int ij = lBorder(j), rbj = rBorder(j); ij < rbj; ij++)
          luMatrix[ij][ii] /= luMatrix[ii][ii];
      }
    }

  }

}

// faster but not determined
template<typename Matrix, typename Vector>
void reverseStroke(Matrix& a, Vector& xb, const int size)
{
  // Lv=f
  for (int i = 1; i < size; i++) {
    for (int j = 0; j < i; j++)
      xb[i] -= a[i][j] * xb[j];
  }
  // Ux=v
  for (int i = size - 1; i >= 0; i--) {
    for (int j = i + 1; j < size; j++)
      xb[i] -= a[i][j] * xb[j];
    xb[i] /= a[i][i];
  }
}
/*
 template<class T>
 class BlockVector{
 private:
 T* vector;
 int size;
 int blockSize;
 public:
 int getSize(){return size;}
 int getBlockSize(){return blockSize;}
 T* getVector(){T* v = new T[size]; for(int i = 0; i < size; i++) v[i] = vector[i]; return v;}
 T& operator[](int n) {return vector[n];}
 T& operator()(int i, int j) {return vector[i * blockSize + j];}
 BlockVector(int n, int bs){vector = new T[n]; size = n; blockSize = bs;}
 ~BlockVector(){delete[] vector;}
 };

 template <typename N>
 N* reverseStrokeBlock(Block<Block<N>> luMatrix, N* f, int size) {
 //we don't want change f
 BlockVector<N> v(size, luMatrix(0, 0).getHeight());
 for (int i = 0; i < size; i++)
 v[i] = f[i];
 // Lv=f
 for (int i = 0; i < luMatrix.getHeight(); i++) {
 for (int j = 0; j < i; j++){
 for (int ii = 0; ii < luMatrix(i, j).getHeight(); ii++)
 for (int jj = 0; jj < luMatrix(i, j).getWidth(); jj++)
 v(i, ii) -= luMatrix(i, j)(ii, jj) * v(j, jj);
 }
 for (int ii = 1; ii < luMatrix(i, i).getHeight(); ii++)
 for (int jj = 0; jj < ii; jj++)
 v(i, ii) -= luMatrix(i, i)(ii, jj) * v(i, jj);
 }
 // Ux=v
 for (int i = luMatrix.getHeight() - 1; i >= 0; i--) {
 for (int j = i + 1; j < luMatrix.getWidth(); j++){
 for (int ii = 0; ii < luMatrix(i, j).getHeight(); ii++)
 for (int jj = 0; jj < luMatrix(i, j).getWidth(); jj++)
 v(i, ii) -= luMatrix(i, j)(ii, jj) * v(j, jj);
 }
 for (int ii = luMatrix(i, i).getHeight() - 1; ii >= 0; ii--) {
 for (int jj = ii + 1; jj < luMatrix(i, i).getWidth(); jj++)
 v(i, ii) -= luMatrix(i, i)(ii, jj) * v(i, jj);
 v(i, ii) /= luMatrix(i, i)[ii][ii];
 }
 }
 return v.getVector();
 }
 */
