#include "s21_matrix.h"

/*Создание матриц (create_matrix)*/

/*
0 - OK
1 - Ошибка, некорректная матрица
2 - Ошибка вычисления (несовпадающие размеры матриц; матрица, для которой нельзя
провести вычисления и т.д.)
*/

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = 0;
  if (result != NULL) {
    if ((rows >= 1 && columns >= 1)) {
      result->rows = rows;
      result->columns = columns;
      result->matrix = calloc(result->rows, sizeof(double *));
      if (result->matrix) {
        for (int i = 0; i < result->rows; i++) {
          result->matrix[i] = calloc(result->columns, sizeof(double));
        }
      }
    } else {
      error = 1;
    }
  }
  return error;
}
