#ifndef PHASE1_PHASE1_H
#define PHASE1_PHASE1_H

void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result);
void multiply_mv_col_major(const double* matrix, int rows, int cols, const double* vector, double* result);
void multiply_mm_naive(const double* matrixA, int rowsA, int colsA, const double* matrixB, int rowsB, int colsB, double* result);
void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA, const double* matrixB_transposed, int rowsB, int colsB, double* result);
void optimized_multiply_mm_naive(const double* matrixA, int rowsA, int colsA, const double* matrixB, int rowsB, int colsB, double* result);
#endif //PHASE1_PHASE1_H
