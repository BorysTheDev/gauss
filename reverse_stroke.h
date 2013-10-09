#ifndef REVERSE_STROKE_H_
#define REVERSE_STROKE_H_

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


#endif /* REVERSE_STROKE_H_ */
