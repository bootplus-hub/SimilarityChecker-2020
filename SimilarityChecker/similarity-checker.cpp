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
};

class LengthChecker : public IChecker {
public:
	static shared_ptr<IChecker> newInstance() {
		return shared_ptr<IChecker>{ new LengthChecker() };
	}

	virtual int score(const string& leftStr, const string& rightStr) const override {
		const int LEFT_LEN = (int)leftStr.length();
		const int RIGHT_LEN = (int)rightStr.length();
		
		if (isPerfectScore(leftStr, rightStr)) return PERFECT_SCOURE;
		
		verifyZeroDivide(LEFT_LEN, RIGHT_LEN);
		if (isZeroScore(LEFT_LEN, RIGHT_LEN)) return 0;
		return (int)(getScoreRate(LEFT_LEN, RIGHT_LEN) * PERFECT_SCOURE);
	}

protected:
	LengthChecker() {}

	virtual bool isZeroScore(int leftLen, int rightLen) const {
		return getZeroRate(leftLen, rightLen) < ZERO_SCOURE_LIMIT;
	}

	virtual bool isPerfectScore(const string& leftStr, const string& rightStr) const {
		return leftStr.length() == rightStr.length();
	}

	virtual void verifyZeroDivide(int leftLen, int rightLen) const {
		if (leftLen && rightLen) return;
		throw invalid_argument("All should not be empty");
	}

	virtual double getScoreRate(int leftLen, int rightLen) const {
		return 1 - abs(leftLen - rightLen) / (double)min(leftLen, rightLen);
	}

	virtual double getZeroRate(int leftLen, int rightLen) const {
		return min(leftLen, rightLen) / (double)max(leftLen, rightLen);
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

	virtual int score(const string& leftStr, const string& rightStr) const override {
		unordered_set<char> left = parsingChars(leftStr)
			, right = parsingChars(rightStr);

		if (isPerfectScore(left, right)) return PERFECT_SCOURE;
		if (isZeroScore(left, right)) return 0;

		return (int)(getRate(left, right) * PERFECT_SCOURE);
	}

protected:
	AlphaChecker() {}

	virtual bool isZeroScore(const unordered_set<char>& left, const unordered_set<char>& right) const {
		for (auto ch : left) {
			if (right.count(ch)) return false;
		}
		return true;
	}

	virtual bool isPerfectScore(const unordered_set<char>& left, const unordered_set<char>& right) const {
		return left == right;
	}

	virtual unordered_set<char> parsingChars(const string& str) const {
		unordered_set<char> rst;
		for (char ch : str) rst.insert(ch);
		return rst;
	}

	virtual int getSameCount(const unordered_set<char>& left, const unordered_set<char>& right) const {
		int ans = 0;
		for (char ch : left) {
			ans += (int)right.count(ch);
		}
		return ans;
	}

	virtual int getTotalCount(const unordered_set<char>& left, const unordered_set<char>& right) const {
		unordered_set<char> ans;
		ans.insert(left.begin(), left.end());
		ans.insert(right.begin(), right.end());
		return (int)ans.size();
	}

	virtual double getRate(const unordered_set<char>& left, const unordered_set<char>& right) const {
		return getSameCount(left, right) / (double)getTotalCount(left, right);
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