#include "s21_matrix.h"

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (working_matrix(A)) {
    if (A->rows == A->columns) {
      double trash_determ = 0.;
      if (!s21_determinant(A, &trash_determ) && fabs(trash_determ) > 1e-6) {
        matrix_t calc_trash = {0};
        matrix_t trans_trash = {0};
        if (!s21_calc_complements(A, &calc_trash)) {
          s21_transpose(&calc_trash, &trans_trash);
          if (s21_create_matrix(A->rows, A->columns, result) == 0) {
            for (int i = 0; i < A->rows; i++) {
              for (int j = 0; j < A->columns; j++) {
                result->matrix[i][j] = trans_trash.matrix[i][j] / trash_determ;
              }
            }
          } else {
            res = 1;
          }
        } else {
          res = 1;
        }
        s21_remove_matrix(&calc_trash);
        s21_remove_matrix(&trans_trash);
      } else {
        res = 2;
      }
    } else {
      res = 2;
    }
  } else {
    res = 1;
  }
  return res;
}