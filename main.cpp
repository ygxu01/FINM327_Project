#include <iostream>
#include <stdexcept>
#include <cmath>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include "phase1.h"
// part 1
//int main() {
////     expecting:
////        multiply_mv_row_major: 6 15
////
////        multiply_mv_col_major: 6 15
////
////        multiply_mm_naive: 22 28 49 64
////
////        multiply_mm_transposed_b: 22 28 49 64
//    try {
//        int rowsA = 2, colsA = 3;
//        int rowsB = 3, colsB = 2;
//
//        auto A = new double[6]{1, 2, 3, 4, 5, 6};
//        auto v = new double[3]{1, 1, 1};
//        auto B = new double[6]{1, 2, 3, 4, 5, 6};
//        auto B_T = new double[6]{1, 3, 5, 2, 4, 6};
//        auto A_col_major = new double[6]{1, 4, 2, 5, 3, 6};
//
//        auto result1 = new double[2];
//        multiply_mv_row_major(A, rowsA, colsA, v, result1);
//        std::cout << "multiply_mv_row_major: ";
//        for (int i = 0; i < rowsA; ++i) std::cout << result1[i] << " ";
//        std::cout << "\n";
//
//        auto result2 = new double[2];
//        multiply_mv_col_major(A_col_major, rowsA, colsA, v, result2);
//        std::cout << "multiply_mv_col_major: ";
//        for (int i = 0; i < rowsA; ++i) std::cout << result2[i] << " ";
//        std::cout << "\n";
//
//        auto result3 = new double[4];
//        multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, result3);
//        std::cout << "multiply_mm_naive: ";
//        for (int i = 0; i < rowsA * colsB; ++i) std::cout << result3[i] << " ";
//        std::cout << "\n";
//
//        auto result4 = new double[4];
//        multiply_mm_transposed_b(A, rowsA, colsA, B_T, rowsB, colsB, result4);
//        std::cout << "multiply_mm_transposed_b: ";
//        for (int i = 0; i < rowsA * colsB; ++i) std::cout << result4[i] << " ";
//        std::cout << "\n";
//
//
//        delete[] A;
//        delete[] v;
//        delete[] B;
//        delete[] B_T;
//        delete[] A_col_major;
//        delete[] result1;
//        delete[] result2;
//        delete[] result3;
//        delete[] result4;
//
//    } catch (const std::exception& e) {
//        std::cerr << "Runtime error: " << e.what() << std::endl;
//    }
//
//    return 0;
//
//}


//part 2.1
int main() {
    try {
        std::vector<int> sizes = {64, 128, 256};
        int runs = 1000;

        for (int size : sizes) {
            int rowsA = size, colsA = size;
            int rowsB = size, colsB = size;

            auto A = new double[rowsA * colsA];
            auto v = new double[colsA];
            auto B = new double[rowsB * colsB];
            auto B_T = new double[colsB * rowsB];
            auto A_col_major = new double[rowsA * colsA];
            auto result_mv = new double[rowsA];
            auto result_mm = new double[rowsA * colsB];

            for (int i = 0; i < rowsA * colsA; ++i) A[i] = A_col_major[i] = static_cast<double>(i % 100);
            for (int i = 0; i < colsA; ++i) v[i] = 1.0;
            for (int i = 0; i < rowsB * colsB; ++i) B[i] = static_cast<double>((i * 7) % 100);
            for (int i = 0; i < rowsB; ++i)
                for (int j = 0; j < colsB; ++j)
                    B_T[j * rowsB + i] = B[i * colsB + j];

            // multiply_mv_row_major
            {
                std::vector<double> timings;
                for (int t = 0; t < runs; ++t) {
                    auto start = std::chrono::high_resolution_clock::now();
                    multiply_mv_row_major(A, rowsA, colsA, v, result_mv);
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
                }
                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
                std::cout << "[multiply_mv_row_major] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
            }

            // multiply_mv_col_major
            {
                std::vector<double> timings;
                for (int t = 0; t < runs; ++t) {
                    auto start = std::chrono::high_resolution_clock::now();
                    multiply_mv_col_major(A_col_major, rowsA, colsA, v, result_mv);
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
                }
                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
                std::cout << "[multiply_mv_col_major] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
            }

            // multiply_mm_naive
            {
                std::vector<double> timings;
                for (int t = 0; t < runs; ++t) {
                    auto start = std::chrono::high_resolution_clock::now();
                    multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, result_mm);
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
                }
                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
                std::cout << "[multiply_mm_naive] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
            }

            // multiply_mm_naive
            {
                std::vector<double> timings;
                for (int t = 0; t < runs; ++t) {
                    auto start = std::chrono::high_resolution_clock::now();
                    optimized_multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, result_mm);
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
                }
                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
                std::cout << "[multiply_mm_naive] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
            }


            // multiply_mm_transposed_b
            {
                std::vector<double> timings;
                for (int t = 0; t < runs; ++t) {
                    auto start = std::chrono::high_resolution_clock::now();
                    multiply_mm_transposed_b(A, rowsA, colsA, B_T, rowsB, colsB, result_mm);
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
                }
                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
                std::cout << "[multiply_mm_transposed_b] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
            }



            delete[] A;
            delete[] v;
            delete[] B;
            delete[] B_T;
            delete[] A_col_major;
            delete[] result_mv;
            delete[] result_mm;
        }

    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }

    return 0;
}

// part 2.3
//#include <malloc.h>
//int main() {
//    try {
//        std::vector<int> sizes = {64, 128, 256};
//        int runs = 1000;
//
//        for (int size : sizes) {
//            int rowsA = size, colsA = size;
//            int rowsB = size, colsB = size;
//            size_t align = 64;
//            size_t bytes_matrix = rowsA * colsA * sizeof(double);
//            size_t bytes_vector = colsA * sizeof(double);
//            size_t bytes_result_mv = rowsA * sizeof(double);
//            size_t bytes_matrixB = rowsB * colsB * sizeof(double);
//
//            auto A = static_cast<double*>(_aligned_malloc(bytes_matrix, align));
//            auto v = static_cast<double*>(_aligned_malloc(bytes_vector, align));
//            auto B = static_cast<double*>(_aligned_malloc(bytes_matrixB, align));
//            auto B_T = static_cast<double*>(_aligned_malloc(bytes_matrixB, align));
//            auto A_col_major = static_cast<double*>(_aligned_malloc(bytes_matrix, align));
//            auto result_mv = static_cast<double*>(_aligned_malloc(bytes_result_mv, align));
//            auto result_mm = static_cast<double*>(_aligned_malloc(bytes_matrix, align));
//
//            for (int i = 0; i < rowsA * colsA; ++i) A[i] = A_col_major[i] = static_cast<double>(i % 100);
//            for (int i = 0; i < colsA; ++i) v[i] = 1.0;
//            for (int i = 0; i < rowsB * colsB; ++i) B[i] = static_cast<double>((i * 7) % 100);
//            for (int i = 0; i < rowsB; ++i)
//                for (int j = 0; j < colsB; ++j)
//                    B_T[j * rowsB + i] = B[i * colsB + j];
//
//            // Benchmark multiply_mv_row_major
//            {
//                std::vector<double> timings;
//                for (int t = 0; t < runs; ++t) {
//                    auto start = std::chrono::high_resolution_clock::now();
//                    multiply_mv_row_major(A, rowsA, colsA, v, result_mv);
//                    auto end = std::chrono::high_resolution_clock::now();
//                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
//                }
//                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
//                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
//                std::cout << "[multiply_mv_row_major - aligned] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
//            }
//
//            // Benchmark multiply_mv_col_major
//            {
//                std::vector<double> timings;
//                for (int t = 0; t < runs; ++t) {
//                    auto start = std::chrono::high_resolution_clock::now();
//                    multiply_mv_col_major(A_col_major, rowsA, colsA, v, result_mv);
//                    auto end = std::chrono::high_resolution_clock::now();
//                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
//                }
//                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
//                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
//                std::cout << "[multiply_mv_col_major - aligned] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
//            }
//
//            // Benchmark multiply_mm_naive
//            {
//                std::vector<double> timings;
//                for (int t = 0; t < runs; ++t) {
//                    auto start = std::chrono::high_resolution_clock::now();
//                    multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, result_mm);
//                    auto end = std::chrono::high_resolution_clock::now();
//                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
//                }
//                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
//                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
//                std::cout << "[multiply_mm_naive - aligned] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
//            }
//
//            // Benchmark multiply_mm_transposed_b
//            {
//                std::vector<double> timings;
//                for (int t = 0; t < runs; ++t) {
//                    auto start = std::chrono::high_resolution_clock::now();
//                    multiply_mm_transposed_b(A, rowsA, colsA, B_T, rowsB, colsB, result_mm);
//                    auto end = std::chrono::high_resolution_clock::now();
//                    timings.push_back(std::chrono::duration<double, std::milli>(end - start).count());
//                }
//                double mean = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
//                double stdev = std::sqrt(std::inner_product(timings.begin(), timings.end(), timings.begin(), 0.0) / runs - mean * mean);
//                std::cout << "[multiply_mm_transposed_b - aligned] Size: " << size << "x" << size << ", Avg: " << mean << " ms, Std: " << stdev << " ms\n";
//            }
//
//            _aligned_free(A);
//            _aligned_free(v);
//            _aligned_free(B);
//            _aligned_free(B_T);
//            _aligned_free(A_col_major);
//            _aligned_free(result_mv);
//            _aligned_free(result_mm);
//        }
//
//    } catch (const std::exception& e) {
//        std::cerr << "Runtime error: " << e.what() << std::endl;
//    }
//
//    return 0;
//}
