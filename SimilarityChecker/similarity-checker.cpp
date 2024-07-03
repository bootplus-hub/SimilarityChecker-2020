#include <string>
#include <algorithm>

using namespace std;

class SimilarityChecker {
public:
	int length(const string& leftStr, const string& rightStr) {
		int leftLen = (int)leftStr.length();
		int rightLen = (int)rightStr.length();

		if (leftLen == rightLen) return 60;
		if (min(leftLen, rightLen) / (double)max(leftLen, rightLen) < 0.5) return 0;
		return (int)((1 - abs(leftLen - rightLen) / (double)min(leftLen, rightLen)) * 60);
	}
};