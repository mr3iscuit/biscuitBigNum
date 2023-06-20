#include <bits/stdc++.h>
#include <cstdint>
#include <random>
#include <string.h>
#include <vector>

typedef std::vector<int32_t> Digits;

struct BigNum;
struct PairIterator;
void shift_left(BigNum& a, int8_t shift);
void shift_right(BigNum& a, int8_t shift);

struct PairIterator {
    PairIterator(const Digits& a, const Digits& b) {
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
    Digits::const_iterator a;  // Use const_iterator instead of iterator
    Digits::const_iterator b;  // Use const_iterator instead of iterator
    Digits::const_iterator ea; // Use const_iterator instead of iterator
    Digits::const_iterator eb; // Use const_iterator instead of iterator
};

enum Sign {
	POSITIVE,
	NEGATIVE,
};

struct BigNum {
	Digits _digits;
	Sign sign = POSITIVE;
};

void normalize(BigNum& a) {
	int8_t i = 0;
	for(; i<a._digits.size() and a._digits[i] == 0; i++);
	shift_right(a, i);
}

void swap_sign_with_oter(BigNum& a, BigNum& b) {
	Sign tempSign = a.sign;
	a.sign = b.sign;
	b.sign = tempSign;
}

void abs(BigNum& a) {
	a.sign = POSITIVE;
}

void shift_left(BigNum& a, int8_t shift) {
	Digits res(a._digits.size() + shift);
	std::copy(a._digits.begin(), a._digits.end(), res.begin() + shift);
	a._digits = res;
}

void shift_right(BigNum& a, int8_t shift) {
	if(shift >= a._digits.size() || a._digits.size() == 0) {
		a._digits.resize(1);
		a._digits = { 0 };
		return;
	}
	
	Digits res(a._digits.size() - shift);
	std::copy(a._digits.begin() + shift, a._digits.end(), res.begin());
	a._digits = res;
}

BigNum newBigNum(int64_t a) {
}

BigNum fromString(std::string a) {
	Sign sign = POSITIVE;
	std::string digits;

	if (a[0] == '-') {
		sign = NEGATIVE;
		a = a.substr(1, a.size() - 1);
	}

	if(a[0] == '+') {
		a = a.substr(1, a.size() - 1);
	}

	Digits res(a.size());
	Digits::iterator resit = res.begin(); 
	std::string::iterator ait = a.begin();

	while(resit != res.end() && ait != a.end()) {
		*resit++ = *ait++ - '0';
	}

	return { res, sign };
};

void _add(BigNum& a, const BigNum& b) {
	int carry = 0;
	int maxLen = std::max(a._digits.size(), b._digits.size());
	a._digits.resize(maxLen);

	for(int i = 0; i<maxLen; i++) {
		int32_t a_digit = ( i<a._digits.size() ? a._digits[i] : 0 );
		int32_t b_digit = ( i<b._digits.size() ? b._digits[i] : 0 );

		int32_t SUM = a_digit + b_digit + carry;
		a._digits[i] = SUM % 10;
		carry = SUM / 10;
	}

	if (carry > 0) {
		a._digits.push_back(carry); // WTF? check if carry > 9
	}
}

void _subtract(BigNum& a, const BigNum& b) {
	int32_t borrow = 0;
	for(int i = 0; i < a._digits.size(); i++) {
		int32_t b_digit = ( i<b._digits.size() ? b._digits[i] : 0 );
		int32_t diff = a._digits[i] - b_digit - borrow;

		if(diff < 0) {
			diff += 10;
			borrow = 1;
		} else {
			borrow = 0;
		}

		a._digits[i] = diff;
	}

	normalize(a);

	if(a._digits.size() == 0) {
		a._digits.push_back(0);
	}
}

void _multiply_by_int(BigNum& a, int b) {
	int32_t carry = 0;

	for(auto& digit : a._digits) {
		int product = digit * b + carry;
		digit = product % 10;
		carry = product / 10;
	}

	while(carry > 0) {
		a._digits.push_back(carry % 10);
		carry /= 10;
	}

	normalize(a);
}

std::istream& operator>>(std::istream& is, BigNum& bigNum) {
	std::string str;
	is >> str;

	bigNum = fromString(str);

	return is;
}

std::ostream& operator<<(std::ostream& os, const BigNum& bigNum) {
	os << (bigNum.sign == NEGATIVE ? "-" : "");
	// optimize using std::copy to string
	for(auto d : bigNum._digits) 
		os << d;
	return os;
}

// Relational operators

bool operator==(const BigNum& a, const BigNum& b) {
	if(a.sign != b.sign) return false;
	if(a._digits.size() != b._digits.size()) return false;

	for(auto ab = PairIterator(a._digits, b._digits); ab.notEnd(); ab.next()) {
		if(*ab.a != *ab.b) return false;
	}

	return true;
}

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

int main() {
	std::string input;
	std::cin >> input;

	BigNum a = fromString(input);

	std::cin >> input;
	BigNum b = fromString(input);

	std::cout << a << " " << b << std::endl;
	_add(a, b);
	normalize(a);
	std::cout << a << " " << b << std::endl;
}
