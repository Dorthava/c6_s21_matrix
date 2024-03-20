#include "s21_matrix.h"

int working_matrix(matrix_t *value) {
  int result = 0;
  if (value != NULL) {
    if (value->columns > 0 && value->rows > 0) {
      result = 1;
    }
  }
  return result;
}

int minor_block(matrix_t *A, matrix_t *result) {
  int res = 0;
  if (A->rows == 1) {
    result->matrix[0][0] = 1;
  } else {
    matrix_t minor = {0};
    if (s21_create_matrix(A->rows, A->columns, &minor) == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          finding_the_order(i, j, A, &minor);
          if (pow((-1), (i + j)) < 0) {
            result->matrix[i][j] =
                determinant_calc(NEG_SIGN, &minor, minor.rows - 1);
          } else {
            result->matrix[i][j] =
                determinant_calc(POS_SIGN, &minor, minor.rows - 1);
          }
        }
      }
      s21_remove_matrix(&minor);
    } else {
      res = 1;
    }
  }
  return res;
}

double determinant_calc(int sign, matrix_t *minor, int arr_size) {
  double res = 0.;
  if (arr_size != 1) {
    matrix_t trash = {0};
    if (s21_create_matrix(arr_size, arr_size, &trash) == 0) {
      for (int i = 0; i < arr_size; i++) {
        finding_the_order(0, i, minor, &trash);
        if (i % 2 == 0) {
          res = res + (minor->matrix[0][i] *
                       determinant_calc(POS_SIGN, &trash, arr_size - 1));
        } else {
          res = res + (minor->matrix[0][i] *
                       determinant_calc(NEG_SIGN, &trash, arr_size - 1));
        }
      }
    }
    s21_remove_matrix(&trash);
  } else {
    res = minor->matrix[0][0];
  }
  if (sign == NEG_SIGN) {
    res = -res;
  }
  return res;
}

void finding_the_order(int pos_rows, int pos_colums, matrix_t *A,
                       matrix_t *result) {
  int res_rows = 0, res_colums = 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (i != pos_rows && j != pos_colums) {
        result->matrix[res_rows][res_colums] = A->matrix[i][j];
        res_colums = (res_colums == (A->rows - 1)) ? 0 : res_colums + 1;
        if (res_colums == A->rows - 1) {
          res_rows += 1;
          res_colums = 0;
        }
      }
    }
  }
}