#include <gtest/gtest.h>

#include "TMatrix.h"

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
