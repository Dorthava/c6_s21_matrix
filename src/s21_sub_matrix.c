#include "s21_matrix.h"

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = 0;
  if (working_matrix(A) && working_matrix(B)) {
    if (A->columns == B->columns && A->rows == B->rows) {
      if (!s21_create_matrix(A->rows, A->columns, result)) {
        for (int i = 0; i < result->rows; i++) {
          for (int j = 0; j < result->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
          }
        }
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