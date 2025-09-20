#include <gtest/gtest.h>
#include "Geometry/RectangularMatrix.hpp"

// Тесты конструкторов
TEST(RectangularMatrixTest, DefaultConstructor) {
    RectangularMatrix<int, 2, 2> matrix;
    EXPECT_EQ(matrix.at(0, 0), 0);
    EXPECT_EQ(matrix.at(0, 1), 0);
    EXPECT_EQ(matrix.at(1, 0), 0);
    EXPECT_EQ(matrix.at(1, 1), 0);
}

TEST(RectangularMatrixTest, ValueConstructor) {
    RectangularMatrix<int, 2, 2> matrix(5);
    EXPECT_EQ(matrix.at(0, 0), 5);
    EXPECT_EQ(matrix.at(0, 1), 5);
    EXPECT_EQ(matrix.at(1, 0), 5);
    EXPECT_EQ(matrix.at(1, 1), 5);
}

TEST(RectangularMatrixTest, InitializerListConstructor) {
    RectangularMatrix<int, 2, 2> matrix{1, 2, 3, 4};
    EXPECT_EQ(matrix.at(0, 0), 1);
    EXPECT_EQ(matrix.at(0, 1), 2);
    EXPECT_EQ(matrix.at(1, 0), 3);
    EXPECT_EQ(matrix.at(1, 1), 4);
}

TEST(RectangularMatrixTest, IteratorConstructor) {
    std::vector<int> data = {1, 2, 3, 4};
    RectangularMatrix<int, 2, 2> matrix(data.begin(), data.end());
    EXPECT_EQ(matrix.at(0, 0), 1);
    EXPECT_EQ(matrix.at(0, 1), 2);
    EXPECT_EQ(matrix.at(1, 0), 3);
    EXPECT_EQ(matrix.at(1, 1), 4);
}

// Тесты операций с rvalue
TEST(RectangularMatrixTest, MoveConstructor) {
    RectangularMatrix<int, 2, 2> matrix1{1, 2, 3, 4};
    RectangularMatrix<int, 2, 2> matrix2(std::move(matrix1));
    
    EXPECT_EQ(matrix2.at(0, 0), 1);
    EXPECT_EQ(matrix2.at(0, 1), 2);
    EXPECT_EQ(matrix2.at(1, 0), 3);
    EXPECT_EQ(matrix2.at(1, 1), 4);
}

TEST(RectangularMatrixTest, MoveAssignment) {
    RectangularMatrix<int, 2, 2> matrix1{1, 2, 3, 4};
    RectangularMatrix<int, 2, 2> matrix2;
    matrix2 = std::move(matrix1);
    
    EXPECT_EQ(matrix2.at(0, 0), 1);
    EXPECT_EQ(matrix2.at(0, 1), 2);
    EXPECT_EQ(matrix2.at(1, 0), 3);
    EXPECT_EQ(matrix2.at(1, 1), 4);
}

// Тесты умножения с разными типами данных
TEST(RectangularMatrixTest, MixedTypeMultiplication) {
    RectangularMatrix<int, 2, 2> matrix1{1, 2, 3, 4};
    RectangularMatrix<double, 2, 2> matrix2{1.5, 2.5, 3.5, 4.5};
    
    auto result = matrix1 * matrix2;
    
    EXPECT_DOUBLE_EQ(result.at(0, 0), 1*1.5 + 2*3.5);
    EXPECT_DOUBLE_EQ(result.at(0, 1), 1*2.5 + 2*4.5);
    EXPECT_DOUBLE_EQ(result.at(1, 0), 3*1.5 + 4*3.5);
    EXPECT_DOUBLE_EQ(result.at(1, 1), 3*2.5 + 4*4.5);
    EXPECT_DOUBLE_EQ(result.get_rows(), 2);
    EXPECT_DOUBLE_EQ(result.get_columns(), 2);
}

// Тесты транспонирования
TEST(RectangularMatrixTest, Transpose1x1) {
    RectangularMatrix<int, 1, 1> matrix{5};
    auto transposed = matrix.transposed();
    
    EXPECT_EQ(transposed.at(0, 0), 5);
    EXPECT_EQ(transposed.get_rows(), 1);
    EXPECT_EQ(transposed.get_columns(), 1);
}

TEST(RectangularMatrixTest, Transpose2x3) {
    RectangularMatrix<int, 2, 3> matrix{1, 2, 3, 4, 5, 6};
    auto transposed = matrix.transposed();
    
    EXPECT_EQ(transposed.get_rows(), 3);
    EXPECT_EQ(transposed.get_columns(), 2);
    
    EXPECT_EQ(transposed.at(0, 0), 1);
    EXPECT_EQ(transposed.at(0, 1), 4);
    EXPECT_EQ(transposed.at(1, 0), 2);
    EXPECT_EQ(transposed.at(1, 1), 5);
    EXPECT_EQ(transposed.at(2, 0), 3);
    EXPECT_EQ(transposed.at(2, 1), 6);
}

// Тесты арифметических операций
TEST(RectangularMatrixTest, Addition) {
    RectangularMatrix<int, 2, 2> matrix1{1, 2, 3, 4};
    RectangularMatrix<int, 2, 2> matrix2{5, 6, 7, 8};
    
    auto result = matrix1 + matrix2;
    
    EXPECT_EQ(result.at(0, 0), 6);
    EXPECT_EQ(result.at(0, 1), 8);
    EXPECT_EQ(result.at(1, 0), 10);
    EXPECT_EQ(result.at(1, 1), 12);
}

TEST(RectangularMatrixTest, Subtraction) {
    RectangularMatrix<int, 2, 2> matrix1{5, 6, 7, 8};
    RectangularMatrix<int, 2, 2> matrix2{1, 2, 3, 4};
    
    auto result = matrix1 - matrix2;
    
    EXPECT_EQ(result.at(0, 0), 4);
    EXPECT_EQ(result.at(0, 1), 4);
    EXPECT_EQ(result.at(1, 0), 4);
    EXPECT_EQ(result.at(1, 1), 4);
}

// Тесты итераторов
TEST(RectangularMatrixTest, RowIterators) {
    RectangularMatrix<int, 2, 2> matrix{1, 2, 3, 4};
    
    auto it = matrix.row_begin(0);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(it + 1), 2);
    
    auto it2 = matrix.row_begin(1);
    EXPECT_EQ(*it2, 3);
    EXPECT_EQ(*(it2 + 1), 4);
}

TEST(RectangularMatrixTest, ColumnIterators) {
    RectangularMatrix<int, 2, 2> matrix{1, 2, 3, 4};
    
    auto it = matrix.column_begin(0);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(it + 1), 3);
    
    auto it2 = matrix.column_begin(1);
    EXPECT_EQ(*it2, 2);
    EXPECT_EQ(*(it2 + 1), 4);
}
