#include <gtest/gtest.h>

#include "TVector.h"

#include <sstream>

TEST(TVector, ConstructsAndIndexes) {
    TVector<int> vector{1, 2, 3};
    EXPECT_EQ(vector.get_size(), 3u);
    EXPECT_EQ(vector[0], 1);
    EXPECT_EQ(vector[2], 3);
}

TEST(TVector, SupportsOriginalInsertionOperations) {
    TVector<int> vector{2, 3};
    vector.push_front(1);
    vector.push_back(5);
    vector.insert(4, 3);
    EXPECT_EQ(vector, (TVector<int>{1, 2, 3, 4, 5}));
}

TEST(TVector, SupportsOriginalRemovalOperations) {
    TVector<int> vector{1, 2, 3, 4, 5};
    vector.pop_front();
    vector.pop_back();
    vector.erase(1);
    EXPECT_EQ(vector, (TVector<int>{2, 4}));
}

TEST(TVector, KeepsLogicalOrderWhenBufferWraps) {
    TVector<int> vector;
    for (int value = 1; value <= 14; ++value) vector.push_back(value);
    vector.pop_front();
    vector.push_back(15);
    EXPECT_EQ(vector.front(), 2);
    EXPECT_EQ(vector.back(), 15);
    for (std::size_t i = 0; i < vector.size(); ++i) EXPECT_EQ(vector[i], static_cast<int>(i + 2));
}

TEST(TVector, CopiesIndependently) {
    TVector<int> first{1, 2, 3};
    TVector<int> second = first;
    second[0] = 99;
    EXPECT_EQ(first[0], 1);
    EXPECT_EQ(second[0], 99);
}

TEST(TVector, ReadsAndWritesStreams) {
    TVector<int> vector;
    std::istringstream input("4 10 20 30 40");
    input >> vector;
    EXPECT_EQ(vector, (TVector<int>{10, 20, 30, 40}));
    std::ostringstream output;
    output << vector;
    EXPECT_EQ(output.str(), "{ 10, 20, 30, 40 }");
}

TEST(TVector, CheckedAccessThrows) {
    TVector<int> vector{1};
    EXPECT_THROW(vector.at(1), std::out_of_range);
    TVector<int> empty;
    EXPECT_THROW(empty.front(), std::logic_error);
}
