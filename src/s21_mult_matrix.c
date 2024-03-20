#include "s21_matrix.h"

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if ((working_matrix(A) ^ working_matrix(B)) ||
      (working_matrix(A) == 0 && working_matrix(B) == 0)) {
    res = 1;
  }
  if ((A->rows != B->columns) || (A->columns != B->rows)) {
    res = 2;
  }
  if (s21_create_matrix(A->rows, B->columns, result) == 0) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  } else {
    res = 1;
  }
  return res;
}