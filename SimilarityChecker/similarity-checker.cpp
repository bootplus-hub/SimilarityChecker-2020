#include <string>
#include <algorithm>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

#define interface struct

using namespace std;

interface IChecker {
	virtual int score(const string& leftStr, const string& rightStr) const = 0;
	virtual bool isZeroScore(const string& leftStr, const string& rightStr) const = 0;
	virtual bool isPerfectScore(const string& leftStr, const string& rightStr) const = 0;
	virtual int getPerfectScoure() const = 0;
};

class LengthChecker : public IChecker {
public:
	static shared_ptr<IChecker> newInstance() {
		return shared_ptr<IChecker>{ new LengthChecker() };
	}

	virtual int score(const string& leftStr, const string& rightStr) const {
		const int LEFT_LEN = (int)leftStr.length();
		const int RIGHT_LEN = (int)rightStr.length();
		verifyZeroDivide(LEFT_LEN, RIGHT_LEN);
		return (int)((1 - abs(LEFT_LEN - RIGHT_LEN) / (double)min(LEFT_LEN, RIGHT_LEN)) * PERFECT_SCOURE);
	}

	virtual bool isZeroScore(const string& leftStr, const string& rightStr) const {
		const int LEFT_LEN = (int)leftStr.length();
		const int RIGHT_LEN = (int)rightStr.length();
		verifyZeroDivide(LEFT_LEN, RIGHT_LEN);
		return min(LEFT_LEN, RIGHT_LEN) / (double)max(LEFT_LEN, RIGHT_LEN) < ZERO_SCOURE_LIMIT;
	}

	virtual bool isPerfectScore(const string& leftStr, const string& rightStr) const {
		return leftStr.length() == rightStr.length();
	}

	virtual int getPerfectScoure() const {
		return PERFECT_SCOURE;
	}

protected:
	LengthChecker() {}

	virtual void verifyZeroDivide(int leftLen, int rightLen) const {
		if (leftLen && rightLen) return;
		throw invalid_argument("All should not be empty");
	}

private:
	static constexpr int PERFECT_SCOURE = 60;
	static constexpr double ZERO_SCOURE_LIMIT = 0.5;
};

class AlphaChecker : public IChecker {
public:
	static shared_ptr<IChecker> newInstance() {
		return shared_ptr<IChecker>{ new AlphaChecker() };
	}

	virtual int score(const string& leftStr, const string& rightStr) const {
		unordered_set<char> left = parsingChars(leftStr)
			, right = parsingChars(rightStr)
			, total;
		total.insert(left.begin(), left.end());
		total.insert(right.begin(), right.end());

		int sameCnt = 0;
		for (char ch : left) {
			sameCnt += (int)right.count(ch);
		}
		return (int)(sameCnt / (double)total.size() * PERFECT_SCOURE);
	}

	virtual bool isZeroScore(const string& leftStr, const string& rightStr) const {
		unordered_set<char> left = parsingChars(leftStr)
			, right = parsingChars(rightStr);
		
		for (auto ch : left) {
			if (right.count(ch)) return false;
		}
		return true;
	}

	virtual bool isPerfectScore(const string& leftStr, const string& rightStr) const {
		unordered_set<char> left = parsingChars(leftStr)
			, right = parsingChars(rightStr);
		return left == right;
	}

	virtual int getPerfectScoure() const {
		return PERFECT_SCOURE;
	}

protected:
	AlphaChecker() {}

	virtual unordered_set<char> parsingChars(const string& str) const {
		unordered_set<char> rst;
		for (char ch : str) rst.insert(ch);
		return rst;
	}

private:
	static constexpr int PERFECT_SCOURE = 40;
};


class CheckerFactory {
public:
	enum CheckerType {
		Length, Alpha
	};

	static CheckerFactory& getInstance() {
		static CheckerFactory instance;
		return instance;
	}

	shared_ptr<IChecker> getChecker(CheckerType checkerType) const {
		auto entry = _store.find(checkerType);
		if (entry == _store.end()) throw invalid_argument("Type not supported");
		return entry->second;
	}

protected:
	CheckerFactory() {
		addChecker(Length, LengthChecker::newInstance());
		addChecker(Alpha, AlphaChecker::newInstance());
	}

	CheckerFactory(const CheckerFactory&) = delete;
	CheckerFactory& operator =(const CheckerFactory&) = delete;

	virtual void addChecker(CheckerType checkerType, shared_ptr<IChecker> checkerPtr) {
		_store[checkerType] = checkerPtr;
	}
private:
	unordered_map<CheckerType, shared_ptr<IChecker>> _store;
};

class SimilarityChecker {
public:
	void injectChecker(const shared_ptr<IChecker>& checker) {
		_checker = checker;
	}

	int score(const string& leftStr, const string& rightStr) {
		if (checker().isPerfectScore(leftStr, rightStr)) return checker().getPerfectScoure();
		if (checker().isZeroScore(leftStr, rightStr)) return 0;
		return checker().score(leftStr, rightStr);
	}

protected:
	IChecker& checker() const {
		if (_checker.get() == nullptr) throw runtime_error("Please inject the checker first");
		return *_checker;
	}

private:
	shared_ptr<IChecker> _checker;
};