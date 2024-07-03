#include <string>

using namespace std;

class SimilarityChecker {
public:
	int length(const string& leftStr, const string& rightStr) {
		if (leftStr.length() == rightStr.length()) return 60;
		return 0;
	}
};