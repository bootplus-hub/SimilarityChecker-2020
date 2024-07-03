#include "pch.h"
#include "../SimilarityChecker/similarity-checker.cpp"
#include <string>

using namespace std;

TEST(SimilarityCheckerLength, Length60) {
	SimilarityChecker checker;
	EXPECT_EQ(60, checker.length("ASD", "DSA"));
}

TEST(SimilarityCheckerLength, Length0) {
	SimilarityChecker checker;
	EXPECT_EQ(0, checker.length("A", "BB"));
}

TEST(SimilarityCheckerLength, LengthOther) {
	SimilarityChecker checker;
	EXPECT_EQ(20, checker.length("AAABB", "BAA"));
}