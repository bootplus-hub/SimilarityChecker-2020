#include <string>
#include <algorithm>

using namespace std;

class SimilarityChecker {
public:
	int score(const string& leftStr, const string& rightStr) {
		int leftLen = (int)leftStr.length();
		int rightLen = (int)rightStr.length();

		if (isPerfectScore(leftLen, rightLen)) return 60;
		if (isZeroScore(leftLen, rightLen)) return 0;
		return calcScore(leftLen, rightLen);
	}

protected:
	bool isZeroScore(int leftLen, int rightLen) const {
		return min(leftLen, rightLen) / (double)max(leftLen, rightLen) < 0.5;
	}

	bool isPerfectScore(int leftLen, int rightLen) const {
		return leftLen == rightLen;
	}

	int calcScore(int leftLen, int rightLen) const {
		return (int)((1 - abs(leftLen - rightLen) / (double)min(leftLen, rightLen)) * 60);
	}
};