#include "phase1.h"
#include <iostream>

void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result)
{
    // suppose vector must be cols * 1 form as it is matrix * vector calculation

    // (pointer, cannot realize)if number of columns not equal to len of vector, error
    // (pointer, cannot realize)if matrix length deos not meet rows * cols, error
    if (!matrix || !vector || !result)
    {
        throw std::invalid_argument("Null pointer!");
    }
    if (rows <= 0 || cols <= 0)
    {
        throw std::invalid_argument("Invalid matrix dimensions.");
    }

    for (int i = 0; i < rows; ++i)
    {
        double sum = 0.0;
        int values_used = i*cols;
        for (int j = 0; j < cols; ++j)
        {
            sum += matrix[values_used + j ] * vector[j];
        }
        result[i] = sum;
    }
}

void multiply_mv_col_major(const double* matrix, int rows, int cols, const double* vector, double* result)
{
    if (!matrix || !vector || !result)
    {
        throw std::invalid_argument("Null pointer!");
    }
    if (rows <= 0 || cols <= 0)
    {
        throw std::invalid_argument("Invalid matrix dimensions.");
    }

    for (int i = 0; i < rows; ++i)
        result[i] = 0.0;

    for (int j = 0; j < cols; ++j)
    {
        double v_j = vector[j];
        for (int i=0; i < rows; ++i)
        {
           result[i] += matrix[j * rows + i] * v_j;
        }
    }
}

void multiply_mm_naive(const double* matrixA, int rowsA, int colsA, const double* matrixB, int rowsB, int colsB, double* result)
{
    if (!matrixA || !matrixB || !result)
    {
        throw std::invalid_argument("Null pointer");
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0)
    {
        throw std::invalid_argument("Invalid matrix dimensions.");
    }

    if (colsA != rowsB)
    {
        throw std::invalid_argument("A and B fail to multipy (Incompatible dimensions)");
    }

    for (int i = 0; i < rowsA; ++i)
    {
        for (int j = 0; j < colsB; ++j)
        {
            result[i * colsB + j] = 0.0;
        }
    }

    for (int i = 0; i < rowsA; ++i)
    {
        for (int k = 0; k < colsA; ++k)
        {
            double a_ik = matrixA[i * colsA + k];
            for (int j = 0; j < colsB; ++j)
            {
                result[i * colsB + j] += a_ik * matrixB[k * colsB + j];
            }
        }
    }
}


void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA, const double* matrixB_transposed, int rowsB, int colsB, double* result)
{
    if (!matrixA || !matrixB_transposed || !result)
    {
        throw std::invalid_argument("Null pointer");
    }

    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0)
    {
        throw std::invalid_argument("Invalid matrix dimensions.");
    }

    if (colsA != rowsB)
    {
        throw std::invalid_argument("A and B fail to multipy (Incompatible dimensions)");
    }

    for (int i = 0; i < rowsA; ++i)
    {
        for (int j = 0; j < colsB; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < colsA; ++k)
            {
                sum += matrixA[i * colsA + k] * matrixB_transposed[j * colsA + k];
            }
            result[i * colsB + j] = sum;
        }
    }
}



void optimized_multiply_mm_naive(const double* matrixA, int rowsA, int colsA, const double* matrixB, int rowsB, int colsB, double* result) {
    constexpr int BLOCK_SIZE = 64;
    for (int i = 0; i < rowsA; i += BLOCK_SIZE) {
        for (int j = 0; j < colsB; j += BLOCK_SIZE) {
            for (int k = 0; k < colsA; k += BLOCK_SIZE) {
                for (int ii = i; ii < std::min(i + BLOCK_SIZE, rowsA); ++ii) {
                    for (int jj = j; jj < std::min(j + BLOCK_SIZE, colsB); ++jj) {
                        double sum = 0.0;
                        for (int kk = k; kk < std::min(k + BLOCK_SIZE, colsA); ++kk) {
                            sum += matrixA[ii * colsA + kk] * matrixB[kk * colsB + jj];
                        }
                        result[ii * colsB + jj] += sum;
                    }
                }
            }
        }
    }
}