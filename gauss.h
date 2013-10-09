#ifndef GAUSS_H_
#define GAUSS_H_
#include "lu.h"
#include "reverse_stroke.h"

// faster but not determined
template<typename Matrix, typename Vector>
void gaussScheme(Matrix& a, Vector& b, const int size)
{
  simpleLU(a, size);
  reverseStroke(a, b, size);
}

template<typename Matrix, typename Vector>
void gaussBlockScheme(Matrix& a, Vector& b, const int size,
    const int blockSize)
{
  blockLU(a, size, blockSize);
  reverseStroke(a, b, size);
}

#endif /* GAUSS_H_ */
