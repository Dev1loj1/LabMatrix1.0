#include <gtest/gtest.h>

#include "TMathVector.h"

TEST(TMathVector, AddsAndSubtractsVectors) {
    const TMathVector<int> first{1, 2, 3};
    const TMathVector<int> second{4, 5, 6};
    EXPECT_EQ(first + second, (TMathVector<int>{5, 7, 9}));
    EXPECT_EQ(second - first, (TMathVector<int>{3, 3, 3}));
}

TEST(TMathVector, MultipliesAndDividesByScalar) {
    const TMathVector<double> vector{2.0, 4.0};
    EXPECT_EQ(vector * 2.0, (TMathVector<double>{4.0, 8.0}));
    EXPECT_EQ(2.0 * vector, (TMathVector<double>{4.0, 8.0}));
    EXPECT_EQ(vector / 2.0, (TMathVector<double>{1.0, 2.0}));
    EXPECT_THROW(vector / 0.0, std::invalid_argument);
}

TEST(TMathVector, CalculatesDotProductAndLength) {
    const TMathVector<int> first{1, 2, 3};
    const TMathVector<int> second{4, 5, 6};
    EXPECT_EQ(first * second, 32);
    EXPECT_DOUBLE_EQ(TMathVector<int>({3, 4}).length(), 5.0);
}

TEST(TMathVector, RejectsDifferentSizes) {
    const TMathVector<int> short_vector{1, 2};
    const TMathVector<int> long_vector{1, 2, 3};
    EXPECT_THROW(short_vector + long_vector, std::invalid_argument);
    EXPECT_THROW(short_vector.dot(long_vector), std::invalid_argument);
}

TEST(TMathVector, KeepsInheritedTVectorOperations) {
    TMathVector<int> vector{2, 3};
    vector.push_front(1);
    vector.push_back(4);
    EXPECT_EQ(vector.front(), 1);
    EXPECT_EQ(vector.back(), 4);
    EXPECT_EQ(vector.size(), 4u);
}
