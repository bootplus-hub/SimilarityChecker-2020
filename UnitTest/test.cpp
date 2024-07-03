#include "pch.h"
#include "../SimilarityChecker/similarity-checker.cpp"
#include <string>

using namespace std;

TEST(SimilarityCheckerLength, Length60) {
	SimilarityChecker checker;
	EXPECT_EQ(60, checker.length("ASD", "DSA"));
}