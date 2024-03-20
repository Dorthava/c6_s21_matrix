#include "s21_matrix.h"

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (working_matrix(A)) {
    if (A->rows == A->columns) {
      if (s21_create_matrix(A->columns, A->rows, result) == 0) {
        minor_block(A, result);
      } else {
        res = 1;
      }
    } else {
      res = 2;
    }
  } else {
    res = 1;
  }
  return res;
}