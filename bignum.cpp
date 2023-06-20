#include <bits/stdc++.h>
#include <string.h>

struct PairIterator {
    PairIterator(const std::string& a, const std::string& b) {
        this->a = a.begin();
        this->b = b.begin();
        this->ea = a.end();
        this->eb = b.end();
    }
    bool notEnd() {
        return a != ea && b != eb;
    }
    void next() {
        ++a;
        ++b;
    }
    std::string::const_iterator a;  // Use const_iterator instead of iterator
    std::string::const_iterator b;  // Use const_iterator instead of iterator
    std::string::const_iterator ea; // Use const_iterator instead of iterator
    std::string::const_iterator eb; // Use const_iterator instead of iterator
};

enum Sign {
	POSITIVE,
	NEGATIVE,
};

struct BigNum {
	std::string _digits;
	Sign sign = POSITIVE;
};

BigNum newBigNum(int64_t a) {
	std::string digits;
	Sign sign = POSITIVE;

	a = std::abs(a);

	if (a < 0) {
		sign = NEGATIVE;
	}

	if (a == 0) digits = "0";

	digits = std::to_string(a);

	return {digits, sign};
}

BigNum newBigNum(std::string a) {
	Sign sign = POSITIVE;
	std::string digits;

	if (a[0] == '-') {
		sign = NEGATIVE;
		a = a.substr(1, a.size() - 1);
	}

	if (a[0] == '+') {
	}

	return {a, sign};
};

std::istream& operator>>(std::istream& is, BigNum& bigNum) {
	std::string str;
	is >> str;

	bigNum = newBigNum(str);

	return is;
}

std::ostream& operator<<(std::ostream& os, const BigNum& bigNum) {
	os << (bigNum.sign == NEGATIVE ? "-" : "") << bigNum._digits;
	return os;
}

// Relational operators

bool operator==(const BigNum& a, const BigNum& b) {}

bool operator>(const BigNum& a, const BigNum& b) {
	if (a.sign == NEGATIVE && b.sign == POSITIVE) return false;
	if (a.sign == POSITIVE && b.sign == NEGATIVE) return true;

	if (a._digits.size() > b._digits.size()) return true;
	if (a._digits.size() < b._digits.size()) return false;

	for (auto ab = PairIterator(a._digits, b._digits); ab.notEnd();
	     ab.next()) {
		if (*ab.a > *ab.b) return false;
	}

	return true;
}

bool operator<(const BigNum& a, const BigNum b) {
	return !(a == b) && !(a > b);
}

BigNum operator+(const BigNum& a, const BigNum& b) {
	// can be optimized
	BigNum a1 = b > a ? b : a;
	BigNum b1 = b > a ? a : b;


}

int main() { return 0; }
