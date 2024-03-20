#include "s21_matrix.h"

int s21_determinant(matrix_t *A, double *result) {
  int res = 0;
  if (working_matrix(A)) {
    if (A->columns == A->rows) {
      if (A->columns == 1) {
        *result = A->matrix[0][0];
      } else {
        *result = determinant_calc(POS_SIGN, A, A->columns);
      }
    } else {
      res = 2;
    }
  } else {
    res = 1;
  }
  return res;
}