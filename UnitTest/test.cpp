#include "pch.h"
#include "../SimilarityChecker/similarity-checker.cpp"
#include <string>

using namespace std;

class SimilarityCheckerFixture : public testing::Test {
public:
	SimilarityChecker checker;
};

TEST_F(SimilarityCheckerFixture, Length60) {
	EXPECT_EQ(60, checker.score("ASD", "DSA"));
}

TEST_F(SimilarityCheckerFixture, Length0) {
	EXPECT_EQ(0, checker.score("A", "BB"));
}

TEST_F(SimilarityCheckerFixture, LengthOther) {
	EXPECT_EQ(20, checker.score("AAABB", "BAA"));
	EXPECT_EQ(30, checker.score("AA", "AAE"));
}