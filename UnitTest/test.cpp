#include "pch.h"
#include "../SimilarityChecker/similarity-checker.cpp"
#include <string>

using namespace std;

class SimilarityCheckerFixture : public testing::Test {
public:
	SimilarityChecker checker;
};

TEST_F(SimilarityCheckerFixture, RuntimeError) {
	EXPECT_THROW(checker.score("ASD", "DSA"), runtime_error);
}

TEST_F(SimilarityCheckerFixture, Length60) {
	checker.injectChecker(CheckerFactory::getInstance().getChecker(CheckerFactory::Length));

	EXPECT_EQ(60, checker.score("ASD", "DSA"));
}

TEST_F(SimilarityCheckerFixture, Length0) {
	checker.injectChecker(CheckerFactory::getInstance().getChecker(CheckerFactory::Length));

	EXPECT_EQ(0, checker.score("A", "BB"));
}

TEST_F(SimilarityCheckerFixture, LengthOther) {
	checker.injectChecker(CheckerFactory::getInstance().getChecker(CheckerFactory::Length));

	EXPECT_EQ(20, checker.score("AAABB", "BAA"));
	EXPECT_EQ(30, checker.score("AA", "AAE"));
}