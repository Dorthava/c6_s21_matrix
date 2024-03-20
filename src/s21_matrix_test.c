#include "s21_matrix.h"

#include <check.h>

double get_rand(double min, double max) {
  double val = (double)rand() / RAND_MAX;
  return min + val * (max - min);
}

START_TEST(create_matrix) {
  int rows = 4;
  int columns = 4;
  matrix_t matrix_check = {0};
  int status = s21_create_matrix(rows, columns, &matrix_check);
  if (matrix_check.matrix != NULL) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        ck_assert(fabs(matrix_check.matrix[i][j]) < 1e-7);
      }
    }
    ck_assert_int_eq(matrix_check.rows, rows);
    ck_assert_int_eq(matrix_check.columns, columns);
    ck_assert_int_eq(status, 0);
  } else {
    ck_assert_int_eq(status, 2);
  }

  if (status == 0) {
    s21_remove_matrix(&matrix_check);
  }
}

END_TEST

START_TEST(create_no_rows) {
  int rows = 0;
  int cols = 10;
  matrix_t mat = {0};
  int result = s21_create_matrix(rows, cols, &mat);
  ck_assert_int_eq(result, 1);
}

END_TEST

START_TEST(create_no_cols) {
  int rows = 10;
  int cols = 0;
  matrix_t mat = {0};
  int result = s21_create_matrix(rows, cols, &mat);
  ck_assert_int_eq(result, 1);
}

END_TEST

START_TEST(create_neg_col) {
  int rows = 4;
  int columns = -4;
  matrix_t matrix = {0};
  int result = s21_create_matrix(rows, columns, &matrix);

  ck_assert_int_eq(result, 1);

  if (result == 0) s21_remove_matrix(&matrix);
}

END_TEST

START_TEST(create_neg_row) {
  int rows = -4;
  int columns = 4;
  matrix_t matrix = {0};
  int result = s21_create_matrix(rows, columns, &matrix);

  ck_assert_int_eq(result, 1);

  if (result == 0) s21_remove_matrix(&matrix);
}

END_TEST

START_TEST(this_matrix_equ) {
  const int rows = 10;
  const int columns = 10;
  matrix_t check_1 = {0};
  int result_cre_1 = s21_create_matrix(rows, columns, &check_1);
  matrix_t check_2 = {0};
  int result_cre_2 = s21_create_matrix(rows, columns, &check_2);

  for (int i = 0; i < check_1.rows; ++i) {
    for (int j = 0; j < check_1.columns; ++j) {
      double rand_val = get_rand(DBL_MIN, DBL_MAX);
      check_1.matrix[i][j] = rand_val;
      check_2.matrix[i][j] = rand_val;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&check_1, &check_2), SUCCESS);
  if (result_cre_1 == 0) {
    s21_remove_matrix(&check_1);
  }
  if (result_cre_2 == 0) {
    s21_remove_matrix(&check_2);
  }
}

END_TEST

START_TEST(this_matrix_not_equ) {
  const int rows = 10;
  const int columns = 10;
  matrix_t check_1 = {0};
  int result_cre_1 = s21_create_matrix(rows, columns, &check_1);
  matrix_t check_2 = {0};
  int result_cre_2 = s21_create_matrix(rows, columns, &check_2);

  for (int i = 0; i < check_1.rows; ++i) {
    for (int j = 0; j < check_1.columns; ++j) {
      check_1.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX) + 1;
      check_2.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&check_1, &check_2), FAILURE);
  if (result_cre_1 == 0) {
    s21_remove_matrix(&check_1);
  }
  if (result_cre_2 == 0) {
    s21_remove_matrix(&check_2);
  }
}

END_TEST

START_TEST(sum_matrix) {
  const int rows = 10;
  const int columns = 10;
  matrix_t matrix_1 = {0};
  s21_create_matrix(rows, columns, &matrix_1);
  matrix_t matrix_2 = {0};
  s21_create_matrix(rows, columns, &matrix_2);
  matrix_t check_matrix = {0};
  s21_create_matrix(rows, columns, &check_matrix);

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      matrix_1.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
      matrix_2.matrix[i][j] = get_rand(DBL_MIN, DBL_MAX);
      check_matrix.matrix[i][j] = matrix_1.matrix[i][j] + matrix_2.matrix[i][j];
    }
  }
  matrix_t res = {0};

  ck_assert_int_eq(s21_sum_matrix(&matrix_1, &matrix_2, &res), 0);
  ck_assert_int_eq(s21_eq_matrix(&check_matrix, &res), SUCCESS);
  s21_remove_matrix(&matrix_1);
  s21_remove_matrix(&matrix_2);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check_matrix);
}

END_TEST

START_TEST(sum_matrix_pos) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &result_2);

  A.matrix[0][0] = 6.5;
  B.matrix[0][0] = 3.5;
  result_2.matrix[0][0] = 10.0;

  s21_sum_matrix(&A, &B, &result);
  ck_assert(fabs(result.matrix[0][0] - result_2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(sum_matrix_neg) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &result_2);

  A.matrix[0][0] = 6.5;
  B.matrix[0][0] = -3.5;
  result_2.matrix[0][0] = 3.0;

  s21_sum_matrix(&A, &B, &result);
  ck_assert(fabs(result.matrix[0][0] - result_2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(sum_check_error_2) {
  matrix_t A, B, result;
  s21_create_matrix(1, 3, &A);
  s21_create_matrix(2, 1, &B);

  A.matrix[0][0] = 6.1;
  B.matrix[0][0] = 3.1;

  int res = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}

START_TEST(sum_check_null) {
  matrix_t *A = NULL;
  matrix_t *B = NULL;
  matrix_t *result = NULL;
  int res = s21_sum_matrix(A, B, result);
  ck_assert_int_eq(res, 1);
}

END_TEST

START_TEST(sub_test_pos) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &result_2);

  A.matrix[0][0] = 6.1801;
  B.matrix[0][0] = 3.1801;
  result_2.matrix[0][0] = 3.0;

  s21_sub_matrix(&A, &B, &result);

  ck_assert(fabs(result.matrix[0][0] - result_2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(sub_test_neg) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &result_2);

  A.matrix[0][0] = 6.18;
  B.matrix[0][0] = -3.18;
  result_2.matrix[0][0] = 9.36;

  s21_sub_matrix(&A, &B, &result);
  ck_assert(fabs(result.matrix[0][0] - result_2.matrix[0][0]) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(sub_test_1_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(7, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 6.9;
  B.matrix[0][0] = 8.9;

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}

END_TEST

START_TEST(sub_check_null) {
  matrix_t *A = NULL;
  matrix_t *B = NULL;
  matrix_t *result = NULL;
  int res = s21_sub_matrix(A, B, result);
  ck_assert_int_eq(res, 1);
}

END_TEST

START_TEST(mult_2_error) {
  matrix_t A = {0};
  matrix_t B = {0};
  s21_create_matrix(4, 5, &A);
  s21_create_matrix(6, 6, &B);

  matrix_t result = {0};
  int mul_error = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(mul_error, 2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

END_TEST

START_TEST(mult_1_error) {
  matrix_t A = {0};
  matrix_t B = {0};
  s21_create_matrix(5, 5, &A);
  s21_create_matrix(0, 6, &B);

  matrix_t result = {0};
  int mul_code = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(mul_code, 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

END_TEST

START_TEST(mult_matrix_easy) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  s21_create_matrix(1, 1, &result_2);

  A.matrix[0][0] = 3;
  B.matrix[0][0] = 3;
  result_2.matrix[0][0] = 9;

  s21_mult_matrix(&A, &B, &result);

  ck_assert(fabs(result.matrix[0][0] - result_2.matrix[0][0]) < 1e-7);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(mult_matrix_normal) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 3, &B);
  s21_create_matrix(3, 3, &result_2);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 0;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 1;
  A.matrix[2][0] = 1;
  A.matrix[2][1] = 1;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 1;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 1;
  B.matrix[1][2] = 2;

  result_2.matrix[0][0] = 1;
  result_2.matrix[0][1] = 2;
  result_2.matrix[0][2] = 1;
  result_2.matrix[1][0] = 0;
  result_2.matrix[1][1] = 1;
  result_2.matrix[1][2] = 2;
  result_2.matrix[2][0] = 1;
  result_2.matrix[2][1] = 3;
  result_2.matrix[2][2] = 3;

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &result_2), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(mult_matrix_hard) {
  matrix_t A, B, result, result_2;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &result_2);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 1;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 1;
  A.matrix[1][2] = 2;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 0;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 1;
  B.matrix[2][0] = 1;
  B.matrix[2][1] = 1;

  result_2.matrix[0][0] = 2;
  result_2.matrix[0][1] = 3;
  result_2.matrix[1][0] = 2;
  result_2.matrix[1][1] = 3;

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &result_2), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result_2);
  s21_remove_matrix(&result);
}

END_TEST

START_TEST(mult_number_easy_test) {
  matrix_t A, result, result_2;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &result_2);

  A.matrix[0][0] = 0;
  A.matrix[0][1] = 1;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 5;

  result_2.matrix[0][0] = 0;
  result_2.matrix[0][1] = 2;
  result_2.matrix[0][2] = 6;
  result_2.matrix[1][0] = 4;
  result_2.matrix[1][1] = 8;
  result_2.matrix[1][2] = 10;

  s21_mult_number(&A, 2, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &result_2), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(null_mult_num) {
  matrix_t *A = NULL;
  matrix_t *result = NULL;
  double num = 999;
  int res = s21_mult_number(A, num, result);
  ck_assert_int_eq(res, 1);
}

END_TEST

START_TEST(transpose_easy) {
  matrix_t A, result, result_2;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &result_2);

  A.matrix[0][0] = 1.25;
  result_2.matrix[0][0] = 1.25;

  s21_transpose(&A, &result);
  ck_assert_int_eq(s21_eq_matrix(&result, &result_2), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&result_2);
}

END_TEST

START_TEST(transpose_normal) {
  matrix_t A, B, result;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 1;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 3;
  A.matrix[2][0] = 1;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 3;

  s21_transpose(&A, &result);

  B.matrix[0][0] = 1;
  B.matrix[1][0] = 2;
  B.matrix[2][0] = 3;
  B.matrix[0][1] = 1;
  B.matrix[1][1] = 2;
  B.matrix[2][1] = 3;
  B.matrix[0][2] = 1;
  B.matrix[1][2] = 2;
  B.matrix[2][2] = 3;

  ck_assert_int_eq(s21_eq_matrix(&result, &B), 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

END_TEST

START_TEST(transpose_error_1) {
  matrix_t A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  A.columns = -6;

  int res = s21_transpose(&A, &B);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}

END_TEST

START_TEST(compliment_test_in_task) {
  matrix_t A, B, result;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;

  B.matrix[0][0] = 0;
  B.matrix[0][1] = 10;
  B.matrix[0][2] = -20;
  B.matrix[1][0] = 4;
  B.matrix[1][1] = -14;
  B.matrix[1][2] = 8;
  B.matrix[2][0] = -8;
  B.matrix[2][1] = -2;
  B.matrix[2][2] = 4;

  int res = s21_calc_complements(&A, &result);
  ck_assert_int_eq(s21_eq_matrix(&result, &B), SUCCESS);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&A);
}

END_TEST

START_TEST(compliment_test_not_square) {
  matrix_t B, R;
  s21_create_matrix(2, 3, &B);

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[0][2] = 3;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 4;
  B.matrix[1][2] = 2;

  int code = s21_calc_complements(&B, &R);

  ck_assert_int_eq(code, 2);
  s21_remove_matrix(&B);
}

END_TEST

START_TEST(compliment_test) {
  matrix_t A, B, result;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);

  A.matrix[0][0] = 5;

  s21_calc_complements(&A, &result);

  B.matrix[0][0] = 1;

  ck_assert_int_eq(s21_eq_matrix(&result, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

END_TEST

START_TEST(null_compliments) {
  matrix_t *B = NULL;
  matrix_t *R = NULL;
  int res = s21_calc_complements(B, R);
  ck_assert_int_eq(res, 1);
}

END_TEST

START_TEST(s21_calc_compl_1) {
  matrix_t A, B, C;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &C);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 0.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;
  s21_calc_complements(&A, &B);
  C.matrix[0][0] = 0.0;
  C.matrix[0][1] = 10.0;
  C.matrix[0][2] = -20.0;
  C.matrix[1][0] = 4.0;
  C.matrix[1][1] = -14.0;
  C.matrix[1][2] = 8.0;
  C.matrix[2][0] = -8.0;
  C.matrix[2][1] = -2.0;
  C.matrix[2][2] = 4.0;
  int res = s21_eq_matrix(&B, &C);
  ck_assert_int_eq(res, SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}

END_TEST

START_TEST(s21_calc_compl_2) {
  matrix_t a = {0};
  s21_create_matrix(3, 3, &a);
  matrix_t res = {0};
  s21_create_matrix(3, 3, &res);
  matrix_t m;
  res.matrix[0][0] = 0;
  res.matrix[0][1] = 10;
  res.matrix[0][2] = -20;
  res.matrix[1][0] = 4;
  res.matrix[1][1] = -14;
  res.matrix[1][2] = 8;
  res.matrix[2][0] = -8;
  res.matrix[2][1] = -2;
  res.matrix[2][2] = 4;

  a.matrix[0][0] = 1;
  a.matrix[0][1] = 2;
  a.matrix[0][2] = 3;
  a.matrix[1][0] = 0;
  a.matrix[1][1] = 4;
  a.matrix[1][2] = 2;
  a.matrix[2][0] = 5;
  a.matrix[2][1] = 2;
  a.matrix[2][2] = 1;
  s21_calc_complements(&a, &m);
  ck_assert_int_eq(s21_eq_matrix(&res, &m), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&a);
  s21_remove_matrix(&res);
}

END_TEST

START_TEST(empty) {
  matrix_t a = {0};
  matrix_t b = {0};
  ck_assert_int_eq(s21_calc_complements(&a, &b), 1);
}

END_TEST

START_TEST(size_mismatch) {
  matrix_t a = {0};
  matrix_t b = {0};
  s21_create_matrix(1, 2, &a);
  ck_assert_int_eq(s21_calc_complements(&a, &b), 2);
  s21_remove_matrix(&a);
}

END_TEST

START_TEST(complements) {
  const int size = 3;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);
  m.matrix[0][0] = 1;
  m.matrix[0][1] = 2;
  m.matrix[0][2] = 3;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 2;
  m.matrix[2][0] = 5;
  m.matrix[2][1] = 2;
  m.matrix[2][2] = 1;

  matrix_t expected = {0};
  s21_create_matrix(size, size, &expected);
  expected.matrix[0][1] = 10;
  expected.matrix[0][2] = -20;
  expected.matrix[1][0] = 4;
  expected.matrix[1][1] = -14;
  expected.matrix[1][2] = 8;
  expected.matrix[2][0] = -8;
  expected.matrix[2][1] = -2;
  expected.matrix[2][2] = 4;

  matrix_t res = {0};
  s21_calc_complements(&m, &res);

  ck_assert_int_eq(s21_eq_matrix(&expected, &res), SUCCESS);
  s21_remove_matrix(&m);
  s21_remove_matrix(&res);
  s21_remove_matrix(&expected);
}

END_TEST

START_TEST(determinant_test_1) {
  matrix_t A;
  double B, R = 1.25;
  s21_create_matrix(1, 1, &A);

  A.matrix[0][0] = 1.25;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_2) {
  matrix_t A;
  double B, R = -69;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 0.25;
  A.matrix[0][1] = 1.25;
  A.matrix[0][2] = 2.25;
  A.matrix[1][0] = 3.25;
  A.matrix[1][1] = 10;
  A.matrix[1][2] = 5.25;
  A.matrix[2][0] = 6.25;
  A.matrix[2][1] = 7.25;
  A.matrix[2][2] = 8.25;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_3x3_1) {
  matrix_t A;
  double B, R = -2;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 2;
  A.matrix[0][1] = 4;
  A.matrix[0][2] = -6;
  A.matrix[1][0] = -5;
  A.matrix[1][1] = -7;
  A.matrix[1][2] = 5;
  A.matrix[2][0] = 3;
  A.matrix[2][1] = 5;
  A.matrix[2][2] = -6;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_3x3_2) {
  matrix_t A;
  double B, R = 25;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = -3;
  A.matrix[0][1] = 4;
  A.matrix[0][2] = -6;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = -7;
  A.matrix[1][2] = 5;
  A.matrix[2][0] = -4;
  A.matrix[2][1] = 5;
  A.matrix[2][2] = -6;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_3x3_3) {
  matrix_t A;
  double B, R = 23;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = -3;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = -6;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = -5;
  A.matrix[1][2] = 5;
  A.matrix[2][0] = -4;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = -6;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_3x3_4) {
  matrix_t A;
  double B, R = -8;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = -3;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 4;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = -5;
  A.matrix[1][2] = -7;
  A.matrix[2][0] = -4;
  A.matrix[2][1] = 3;
  A.matrix[2][2] = 5;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_4x4) {
  matrix_t A;
  double B, R = 18;
  s21_create_matrix(4, 4, &A);

  A.matrix[0][0] = 3;
  A.matrix[0][1] = -3;
  A.matrix[0][2] = -5;
  A.matrix[0][3] = 8;
  A.matrix[1][0] = -3;
  A.matrix[1][1] = 2;
  A.matrix[1][2] = 4;
  A.matrix[1][3] = -6;
  A.matrix[2][0] = 2;
  A.matrix[2][1] = -5;
  A.matrix[2][2] = -7;
  A.matrix[2][3] = 5;
  A.matrix[3][0] = -4;
  A.matrix[3][1] = 3;
  A.matrix[3][2] = 5;
  A.matrix[3][3] = -6;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_5x5) {
  matrix_t A;
  double B, R = -69.0 / 4.0;
  s21_create_matrix(5, 5, &A);

  A.matrix[0][0] = 0.25;
  A.matrix[0][1] = 1.25;
  A.matrix[0][2] = 2.25;
  A.matrix[0][3] = 3.25;
  A.matrix[0][4] = 2.25;
  A.matrix[1][0] = 3.25;
  A.matrix[1][1] = 10;
  A.matrix[1][2] = 5.25;
  A.matrix[1][3] = 5.25;
  A.matrix[1][4] = 5.25;
  A.matrix[2][0] = 6.25;
  A.matrix[2][1] = 7.25;
  A.matrix[2][2] = 8.25;
  A.matrix[2][3] = 6.25;
  A.matrix[2][4] = 6.25;
  A.matrix[3][0] = 6.25;
  A.matrix[3][1] = 7.25;
  A.matrix[3][2] = 8.25;
  A.matrix[3][3] = 6.25;
  A.matrix[3][4] = 7.25;
  A.matrix[4][0] = 6.25;
  A.matrix[4][1] = 7.25;
  A.matrix[4][2] = 8.25;
  A.matrix[4][3] = 6;
  A.matrix[4][4] = 8.25;

  s21_determinant(&A, &B);

  ck_assert(fabs(B - R) < 1e-7);

  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_not_square) {
  matrix_t A;
  double B;
  s21_create_matrix(4, 5, &A);
  int res = s21_determinant(&A, &B);
  ck_assert_int_eq(res, 2);
  s21_remove_matrix(&A);
}

END_TEST

START_TEST(determinant_test_4x4_2) {
  matrix_t a;
  double number;
  s21_create_matrix(4, 4, &a);
  a.matrix[0][0] = 1;
  a.matrix[0][1] = 2;
  a.matrix[0][2] = 3;
  a.matrix[0][3] = 13;
  a.matrix[1][0] = 4;
  a.matrix[1][1] = 5;
  a.matrix[1][2] = 6;
  a.matrix[1][3] = 16;
  a.matrix[2][0] = 7;
  a.matrix[2][1] = 8;
  a.matrix[2][2] = 90;
  a.matrix[2][3] = 19;
  a.matrix[3][0] = 1;
  a.matrix[3][1] = 7;
  a.matrix[3][2] = 7;
  a.matrix[3][3] = 17;

  s21_determinant(&a, &number);
  ck_assert_int_eq(number, 13608);
  s21_remove_matrix(&a);
}

END_TEST

START_TEST(null_determinate) {
  matrix_t *B = NULL;
  double re = 0;
  int res = s21_determinant(B, &re);
  ck_assert_int_eq(res, 1);
}

END_TEST

START_TEST(determinant_test_3) {
  const int size = 5;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) m.matrix[i][j] = j;
  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  ck_assert_int_eq(code, 0);

  s21_remove_matrix(&m);
}

END_TEST

START_TEST(determinant_test_4) {
  const int size = 4;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);

  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++) m.matrix[i][j] = j + i;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 0, 1e-6);
  ck_assert_int_eq(code, 0);

  s21_remove_matrix(&m);
}

END_TEST

START_TEST(determinant_test_5) {
  const int size = 5;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);
  m.matrix[0][1] = 6;
  m.matrix[0][2] = -2;
  m.matrix[0][3] = -1;
  m.matrix[0][4] = 5;
  m.matrix[1][3] = -9;
  m.matrix[1][4] = -7;
  m.matrix[2][1] = 15;
  m.matrix[2][2] = 35;
  m.matrix[3][1] = -1;
  m.matrix[3][2] = -11;
  m.matrix[3][3] = -2;
  m.matrix[3][4] = 1;
  m.matrix[4][0] = -2;
  m.matrix[4][1] = -2;
  m.matrix[4][2] = 3;
  m.matrix[4][4] = -2;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 2480, 1e-6);
  ck_assert_int_eq(code, 0);

  s21_remove_matrix(&m);
}

END_TEST

START_TEST(determinant_test_6) {
  const int size = 3;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);
  m.matrix[0][0] = 2;
  m.matrix[0][1] = 3;
  m.matrix[0][2] = 1;
  m.matrix[1][0] = 7;
  m.matrix[1][1] = 4;
  m.matrix[1][2] = 1;
  m.matrix[2][0] = 9;
  m.matrix[2][1] = -2;
  m.matrix[2][2] = 1;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, -32, 1e-6);
  ck_assert_int_eq(code, 0);

  s21_remove_matrix(&m);
}

END_TEST

START_TEST(determinant_test_7) {
  const int size = 2;
  matrix_t m = {0};
  s21_create_matrix(size, size, &m);
  m.matrix[0][0] = -5;
  m.matrix[0][1] = -4;
  m.matrix[1][0] = -2;
  m.matrix[1][1] = -3;

  double res = 0;
  int code = s21_determinant(&m, &res);
  ck_assert_double_eq_tol(res, 7, 1e-6);
  ck_assert_int_eq(code, 0);

  s21_remove_matrix(&m);
}

END_TEST

START_TEST(inverse_easy) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &result);
  result.matrix[0][0] = -2;
  result.matrix[0][1] = 3;
  result.matrix[1][0] = 3;
  result.matrix[1][1] = -4;
  A.matrix[0][0] = 4;
  A.matrix[0][1] = 3;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 2;

  matrix_t B;
  s21_inverse_matrix(&A, &B);
  int res = s21_eq_matrix(&B, &result);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}

END_TEST

START_TEST(inverse_one_by_one) {
  matrix_t m = {0};
  matrix_t result = {0};
  int codec = s21_create_matrix(1, 1, &m);
  if (!codec) {
    m.matrix[0][0] = 1431.12312331;
    int code = s21_inverse_matrix(&m, &result);
    ck_assert_int_eq(result.matrix[0][0] == (1.0 / m.matrix[0][0]), 1);
    ck_assert_int_eq(code, 0);
    s21_remove_matrix(&m);
    s21_remove_matrix(&result);
  }
}

END_TEST

START_TEST(inverse_zero_det) {
  matrix_t m = {0};
  matrix_t result = {0};
  int codec = s21_create_matrix(1, 1, &m);
  if (!codec) {
    int code = s21_inverse_matrix(&m, &result);
    ck_assert_int_eq(code, 2);
    s21_remove_matrix(&m);
  }
}

END_TEST

START_TEST(inverse_not_sqare) {
  matrix_t matrixx = {0};
  matrix_t result = {0};
  int codec = s21_create_matrix(1, 4, &matrixx);
  if (!codec) {
    int code = s21_inverse_matrix(&matrixx, &result);
    ck_assert_int_eq(code, 2);
    s21_remove_matrix(&matrixx);
  }
}

END_TEST

START_TEST(inverse_normal) {
  matrix_t m = {0};
  matrix_t expected = {0};
  int codec1, codec2;
  codec1 = s21_create_matrix(3, 3, &m);
  if (!codec1) codec2 = s21_create_matrix(3, 3, &expected);

  if (!codec1 && !codec2) {
    m.matrix[0][0] = 2;
    m.matrix[0][1] = 5;
    m.matrix[0][2] = 7;

    m.matrix[1][0] = 6;
    m.matrix[1][1] = 3;
    m.matrix[1][2] = 4;

    m.matrix[2][0] = 5;
    m.matrix[2][1] = -2;
    m.matrix[2][2] = -3;

    expected.matrix[0][0] = 1;
    expected.matrix[0][1] = -1;
    expected.matrix[0][2] = 1;

    expected.matrix[1][0] = -38;
    expected.matrix[1][1] = 41;
    expected.matrix[1][2] = -34;

    expected.matrix[2][0] = 27;
    expected.matrix[2][1] = -29;
    expected.matrix[2][2] = 24;
    matrix_t result = {0};
    int code = s21_inverse_matrix(&m, &result);

    ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);
    ck_assert_int_eq(code, 0);

    s21_remove_matrix(&m);
    s21_remove_matrix(&result);
    s21_remove_matrix(&expected);
  }
}

END_TEST

int _test(void) {
  Suite *suite = suite_create("MATRIX");
  TCase *tCase = tcase_create("MATRIX");
  SRunner *sRunner = srunner_create(suite);
  int countFailTest;

  suite_add_tcase(suite, tCase);
  // s21_create_matrix and s21_remove_matrix
  tcase_add_test(tCase, create_matrix);
  tcase_add_test(tCase, create_no_rows);
  tcase_add_test(tCase, create_no_cols);
  tcase_add_test(tCase, create_neg_col);
  tcase_add_test(tCase, create_neg_row);
  // s21_eq_matrix
  tcase_add_test(tCase, this_matrix_equ);
  tcase_add_test(tCase, this_matrix_not_equ);
  // s21_sum_matrix
  tcase_add_test(tCase, sum_matrix);
  tcase_add_test(tCase, sum_matrix_pos);
  tcase_add_test(tCase, sum_matrix_neg);
  tcase_add_test(tCase, sum_check_error_2);
  tcase_add_test(tCase, sum_check_null);
  // s21_sub_matrix
  tcase_add_test(tCase, sub_test_pos);
  tcase_add_test(tCase, sub_test_neg);
  tcase_add_test(tCase, sub_test_1_matrix);
  tcase_add_test(tCase, sub_check_null);
  // s21_mult_matrix
  tcase_add_test(tCase, mult_2_error);
  tcase_add_test(tCase, mult_1_error);
  tcase_add_test(tCase, mult_matrix_easy);
  tcase_add_test(tCase, mult_matrix_normal);
  tcase_add_test(tCase, mult_matrix_hard);
  // s21_mult_number
  tcase_add_test(tCase, null_mult_num);
  tcase_add_test(tCase, mult_number_easy_test);
  // s21_transpose
  tcase_add_test(tCase, transpose_easy);
  tcase_add_test(tCase, transpose_normal);
  tcase_add_test(tCase, transpose_error_1);
  // s21_calc_complements
  tcase_add_test(tCase, compliment_test_in_task);
  tcase_add_test(tCase, compliment_test_not_square);
  tcase_add_test(tCase, compliment_test);
  tcase_add_test(tCase, null_compliments);
  tcase_add_test(tCase, s21_calc_compl_2);
  tcase_add_test(tCase, s21_calc_compl_1);
  tcase_add_test(tCase, complements);
  tcase_add_test(tCase, empty);
  tcase_add_test(tCase, size_mismatch);
  // s21_determinant
  tcase_add_test(tCase, determinant_test_1);
  tcase_add_test(tCase, determinant_test_2);
  tcase_add_test(tCase, determinant_test_3);
  tcase_add_test(tCase, determinant_test_4);
  tcase_add_test(tCase, determinant_test_5);
  tcase_add_test(tCase, determinant_test_6);
  tcase_add_test(tCase, determinant_test_7);
  tcase_add_test(tCase, null_determinate);
  tcase_add_test(tCase, determinant_test_3x3_1);
  tcase_add_test(tCase, determinant_test_3x3_2);
  tcase_add_test(tCase, determinant_test_3x3_3);
  tcase_add_test(tCase, determinant_test_3x3_4);
  tcase_add_test(tCase, determinant_test_4x4);
  tcase_add_test(tCase, determinant_test_4x4_2);
  tcase_add_test(tCase, determinant_test_5x5);
  tcase_add_test(tCase, determinant_not_square);
  // s21_inverse_matrix
  tcase_add_test(tCase, inverse_easy);
  tcase_add_test(tCase, inverse_normal);
  tcase_add_test(tCase, inverse_not_sqare);
  tcase_add_test(tCase, inverse_zero_det);
  tcase_add_test(tCase, inverse_one_by_one);

  srunner_run_all(sRunner, CK_ENV);
  countFailTest = srunner_ntests_failed(sRunner);
  srunner_free(sRunner);

  return countFailTest == 0 ? 0 : 1;
}

int main() {
  int no_failed = 0;
  no_failed += _test();
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}