#include <vector>
#include <functional>

using namespace std;
template<typename N>
void simpleLU(Matrix<N>& luMatrix)
{
  for (size_t i = 0; i < luMatrix.height(); i++) {
    for (size_t k = 0; k < i; k++)
      for (size_t j = i; j < luMatrix.width(); j++)
        luMatrix[i][j] -= luMatrix[i][k] * luMatrix[k][j];

    for (size_t j = i + 1; j < luMatrix.width(); j++) {
      N sum = 0.0;
      for (size_t k = 0; k < i; k++)
        sum += luMatrix[j][k] * luMatrix[k][i];
      luMatrix[j][i] = (luMatrix[j][i] - sum) / luMatrix[i][i];
    }
  }
}

template<typename N>
void blockLU(Matrix<N>& luMatrix, const int bs)
{
  const int size = luMatrix.height();
  const int bn = (size + (bs - 1)) / bs;

  std::function<int(int)> lBorder = [&](int blockn) {
    return blockn * bs;
  };
  std::function<int(int)> rBorder = [&](int blockn) {
    return (blockn + 1) * bs > size ? size : (blockn + 1) * bs;
  };
  // middle blocks
  for (int i = 0; i < bn; i++) {
    //  full U and L part
    for (int k = 0; k < i; k++)
      for (int ii = lBorder(i); ii < rBorder(i); ii++)
        for (int ik = lBorder(k); ik < rBorder(k); ik++)
          for (int ij = lBorder(i); ij < rBorder(i); ij++)
            luMatrix(ii, ij) -= luMatrix(ii, ik) * luMatrix(ik, ij);
    // U and L part last block
    for (int ii = lBorder(i); ii < rBorder(i); ii++) {
      for (int ik = lBorder(i); ik < ii; ik++) {
        for (int ij = ii; ij < rBorder(i); ij++)
          luMatrix(ii, ij) -= luMatrix(ii, ik) * luMatrix(ik, ij);
        for (int ij = ii + 1; ij < rBorder(i); ij++)
          luMatrix(ij, ii) -= luMatrix(ij, ik) * luMatrix(ik, ii);
      }
      //  end of L part
      for (int ij = ii + 1; ij < rBorder(i); ij++)
        luMatrix(ij, ii) /= luMatrix(ii, ii);
    }

    //U blocks
    for (int j = i + 1; j < bn; j++) {
      for (int k = 0; k < i; k++) {
        for (int ii = lBorder(i); ii < rBorder(i); ii++)
          for (int ik = lBorder(k); ik < rBorder(k); ik++)
            for (int ij = lBorder(j); ij < rBorder(j); ij++)
              luMatrix(ii, ij) -= luMatrix(ii, ik) * luMatrix(ik, ij);
      }
      // U part last block
      for (int ii = lBorder(i); ii < rBorder(i); ii++)
        for (int ik = lBorder(i); ik < ii; ik++)
          for (int ij = lBorder(j); ij < rBorder(j); ij++)
            luMatrix(ii, ij) -= luMatrix(ii, ik) * luMatrix(ik, ij);
    }


    for (int j = i + 1; j < bn; j++) {
      for (int k = 0; k < i; k++) {
        for (int ii = lBorder(i); ii < rBorder(i); ii++)
          for (int ik = lBorder(k); ik < rBorder(k); ik++)
            for (int ij = lBorder(j); ij < rBorder(j); ij++)
              luMatrix(ij, ii) -= luMatrix(ij, ik)
                  * luMatrix(ik, ii);
      }
      for (int ii = lBorder(i); ii < rBorder(i); ii++) {
        for (int ik = lBorder(i); ik < ii; ik++)
          for (int ij = lBorder(j); ij < rBorder(j); ij++)
            luMatrix(ij, ii) -= luMatrix(ij, ik)
                * luMatrix(ik, ii);
        for (int ij = lBorder(j); ij < rBorder(j); ij++)
          luMatrix(ij, ii) /= luMatrix(ii, ii);
      }
    }

  }

}


template<typename N>
N* reverseStroke(Matrix<N>& luMatrix, N* f, int size)
{
  //we don't want change f
  N* v = new N[size];
  for (int i = 0; i < size; i++)
    v[i] = f[i];
  // Lv=f
  for (int i = 1; i < size; i++) {
    for (int j = 0; j < i; j++)
      v[i] -= luMatrix[i][j] * v[j];
  }
  // Ux=v
  for (int i = size - 1; i >= 0; i--) {
    for (int j = i + 1; j < size; j++)
      v[i] -= luMatrix[i][j] * v[j];
    v[i] /= luMatrix[i][i];
  }
  return v;
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
 v(i, ii) /= luMatrix(i, i)(ii, ii);
 }
 }
 return v.getVector();
 }
 */
