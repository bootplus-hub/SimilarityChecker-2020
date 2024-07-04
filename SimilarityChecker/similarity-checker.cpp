#include <string>
#include <algorithm>
#include <memory>
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


class CheckerFactory {
public:
	enum CheckerType {
		Length,
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