#include "pch.h"
#include "../SimilarityChecker/similarity-checker.cpp"
#include <string>

using namespace std;

class SimilarityCheckerFixture : public testing::Test {
public:
	SimilarityChecker checker;
	void injectLength() {
		checker.injectChecker(CheckerFactory::getInstance().getChecker(CheckerFactory::Length));
	}
	void injectAlpha() {
		checker.injectChecker(CheckerFactory::getInstance().getChecker(CheckerFactory::Alpha));
	}
};

TEST_F(SimilarityCheckerFixture, RuntimeError) {
	EXPECT_THROW(checker.score("ASD", "DSA"), runtime_error);
}

TEST_F(SimilarityCheckerFixture, InvalidArgument) {
	injectLength();

	EXPECT_THROW(checker.score("ASD", ""), invalid_argument);
}

TEST_F(SimilarityCheckerFixture, Length60) {
	injectLength();

	EXPECT_EQ(60, checker.score("ASD", "DSA"));
}

TEST_F(SimilarityCheckerFixture, Length0) {
	injectLength();

	EXPECT_EQ(0, checker.score("A", "BB"));
}

TEST_F(SimilarityCheckerFixture, LengthOther) {
	injectLength();

	EXPECT_EQ(20, checker.score("AAABB", "BAA"));
	EXPECT_EQ(30, checker.score("AA", "AAE"));
}

TEST_F(SimilarityCheckerFixture, AlphaPerfact) {
	injectAlpha();

	EXPECT_EQ(40, checker.score("ASD", "DSA"));
	EXPECT_EQ(40, checker.score("AAABB", "BA"));
}

TEST_F(SimilarityCheckerFixture, AlphaZero) {
	injectAlpha();

	EXPECT_EQ(0, checker.score("A", "BB"));
	EXPECT_EQ(0, checker.score("", "BB"));
}

TEST_F(SimilarityCheckerFixture, AlphaOhter) {
	injectAlpha();

	EXPECT_EQ(20, checker.score("A", "AAE"));
	EXPECT_EQ(20, checker.score("E", "AAE"));
}

TEST_F(SimilarityCheckerFixture, AlphaInvalidArgument) {
	injectAlpha();
	
	EXPECT_THROW(checker.score("A", "AaE"), invalid_argument);
}