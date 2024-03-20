CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11
GCOVFLAGS:=-fprofile-arcs -ftest-coverage
LDFLAGS:=-lcheck -lgcov -fprofile-arcs --coverage
ifeq ($(shell uname), Linux)
LDFLAGS +=-pthread -lcheck_pic -lrt -lm -lsubunit
endif

SOURCES:= s21_additional_matrix.c s21_calc_complements.c s21_create_matrix.c s21_determinant.c s21_eq_matrix.c s21_inverse_matrix.c s21_mult_matrix.c s21_mult_number.c s21_remove_matrix.c s21_sub_matrix.c s21_sum_matrix.c s21_transpose.c

OBJECTS = $(addprefix obj/,$(SOURCES:.c=.o))

OBJECTS_GCOV = $(addprefix gcov_obj/,$(SOURCES:.c=.o))

.PHONY: all clean rebuild

all: test gcov_report s21_matrix_gcov.a s21_matrix.a

s21_matrix.a: $(OBJECTS)
	ar -src $@ $(OBJECTS)

s21_matrix_gcov.a: $(OBJECTS_GCOV)
	ar -src $@ $(OBJECTS_GCOV)

test: gcov_obj/s21_matrix_test.o s21_matrix_gcov.a
	$(CC) gcov_obj/s21_matrix_test.o s21_matrix_gcov.a -o $@ $(LDFLAGS) $(GCOVFLAGS)
	./test

gcov_report: test
	lcov -t "test" -o gcov_obj/test.info -c -d gcov_obj/.  
	genhtml gcov_obj/test.info --output-directory report
	open ./report/index.html

gcov_obj/%.o: %.c
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $< -o $@

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

rebuild: clean all

style_check:
	clang-format --style=Google -i *.c *.h

clean:
	rm -rf report
	rm -f obj/*.o
	rm -f gcov_obj/*.o
	rm -f gcov_obj/*.gc*
	rm -f ./test
	rm -f -r out
	rm -f gcov_obj/*.info
	rm -f .DS_Store
	rm -f *.a