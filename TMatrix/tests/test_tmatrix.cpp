#include <gtest/gtest.h>

#include "TMatrix.h"

#include <sstream>

TEST(TMatrixInherited, ProvidesTwoLevelIndexing) {
    TMatrix<int> matrix{{1, 2}, {3, 4}};
    EXPECT_EQ(matrix.get_size(), 2u);
    EXPECT_EQ(matrix[0].get_size(), 2u);
    EXPECT_EQ(matrix[1][0], 3);
    matrix[1][0] = 30;
    EXPECT_EQ(matrix[1][0], 30);
}

TEST(TMatrixInherited, CopiesIndependently) {
    const TMatrix<int> original{{1, 2}, {3, 4}};
    TMatrix<int> copy = original;
    copy[0][0] = 99;
    EXPECT_EQ(original[0][0], 1);
    EXPECT_EQ(copy[0][0], 99);
}

TEST(TMatrixInherited, RejectsNonSquareInitializer) {
    EXPECT_THROW((TMatrix<int>{{1, 2, 3}, {4, 5, 6}}), std::invalid_argument);
}

TEST(TMatrix, AddsAndSubtractsMatrices) {
    const TMatrix<int> first{{1, 2}, {3, 4}};
    const TMatrix<int> second{{5, 6}, {7, 8}};
    EXPECT_EQ(first + second, (TMatrix<int>{{6, 8}, {10, 12}}));
    EXPECT_EQ(second - first, (TMatrix<int>{{4, 4}, {4, 4}}));
}

TEST(TMatrix, MultipliesMatrices) {
    const TMatrix<int> first{{1, 2}, {3, 4}};
    const TMatrix<int> second{{5, 6}, {7, 8}};
    EXPECT_EQ(first * second, (TMatrix<int>{{19, 22}, {43, 50}}));
}

TEST(TMatrix, MultipliesByScalar) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};
    EXPECT_EQ(matrix * 3, (TMatrix<int>{{3, 6}, {9, 12}}));
    EXPECT_EQ(3 * matrix, (TMatrix<int>{{3, 6}, {9, 12}}));
}

TEST(TMatrix, TransposesAndCalculatesTrace) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};
    EXPECT_EQ(matrix.transpose(), (TMatrix<int>{{1, 3}, {2, 4}}));
    EXPECT_EQ(matrix.trace(), 5);
}

TEST(TMatrix, CreatesIdentityMatrix) {
    EXPECT_EQ(TMatrix<int>::identity(3), (TMatrix<int>{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}));
}

TEST(TMatrix, ProvidesCheckedTwoDimensionalAccess) {
    TMatrix<int> matrix(2);
    matrix.at(1, 1) = 7;
    EXPECT_EQ(matrix.at(1, 1), 7);
    EXPECT_THROW(matrix.at(2, 0), std::out_of_range);
    EXPECT_THROW(matrix.at(0, 2), std::out_of_range);
}

TEST(TMatrix, RejectsOperationsWithDifferentOrders) {
    const TMatrix<int> small(2);
    const TMatrix<int> large(3);
    EXPECT_THROW(small + large, std::invalid_argument);
    EXPECT_THROW(small * large, std::invalid_argument);
}

TEST(TMatrix, PrintsRowsInMatrixForm) {
    const TMatrix<int> matrix{{1, 2}, {3, 4}};
    std::ostringstream output;
    output << matrix;
    EXPECT_EQ(output.str(), "[ 1 2 ]\n[ 3 4 ]");
}
