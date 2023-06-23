#include <algorithm>
#include <bits/stdc++.h>
#include <cstdint>
#include <string.h>
#include <vector>

typedef std::vector<int16_t> Digits;

struct BigNum;
struct PairIterator;

void swap_sign(BigNum& a, BigNum& b);
void shift_left(BigNum& a, int8_t shift);
void shift_right(BigNum& a, int8_t shift);
void normalize(BigNum& a);
void abs(BigNum& a);
void swap_digits(BigNum& a, BigNum& b);
void _add(BigNum& a, const BigNum& b);
void _subtract(BigNum& a, const BigNum& b);

template<typename PrimitiveType>
void _multiply_by_int(BigNum& a, PrimitiveType b);

BigNum fromString(std::string a);
BigNum fromInt(int64_t a);

enum class Sign {
	POSITIVE,
	NEGATIVE,
};

struct BigNum {
	Digits _digits;
	Sign sign;
};

void normalize(BigNum& a) {
	int8_t c = 0;

	for(int i = a._digits.size()-1; i != 0; i--) {
		if(a._digits[i] > 0) break;
		c++;
	}

	shift_right(a, c);
	if(a._digits.size() == 0) {
		a._digits.push_back(0);
	}
}

void swap_digits(BigNum& a, BigNum& b) {
	a._digits.swap(b._digits);
}

void swap_sign(BigNum& a, BigNum& b) {
	Sign tempSign = a.sign;
	a.sign = b.sign;
	b.sign = tempSign;
}

void abs(BigNum& a) {
	a.sign = Sign::POSITIVE;
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

BigNum fromInt(int64_t a) {
	BigNum bigNum;
	bigNum.sign = (a < 0 ? Sign::NEGATIVE : Sign::POSITIVE);

	a = std::abs(a);
	while(a) {
		bigNum._digits.push_back(a % 10);
		a /= 10;
	}

	return bigNum;
}

BigNum fromString(std::string a) {
	Sign sign = Sign::POSITIVE;

	if (a[0] == '-') {
		sign = Sign::NEGATIVE;
		a = a.substr(1, a.size() - 1);
	}

	if(a[0] == '+') {
		a = a.substr(1, a.size() - 1);
	}

	Digits res(a.size());
	Digits::iterator      resit = res.end() -1; 
	std::string::iterator ait   = a.begin();

	while(resit != res.begin() -1 && ait != a.end()) {
		*resit-- = *ait++ - '0';
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
}

template<typename PrimitiveType>
void _multiply_by_int(BigNum& a, PrimitiveType b) {
	PrimitiveType carry = 0;

	for(auto& digit : a._digits) {
		PrimitiveType product = digit * b + carry;
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
	os << (bigNum.sign == Sign::NEGATIVE ? "-" : "");
	Digits temp = bigNum._digits;

	std::reverse(temp.begin(), temp.end());
	for(auto d : temp) {
		os << d;
	}

	return os;
}

// Relational operators

bool operator==(const BigNum& a, const BigNum& b) {
	if(a.sign != b.sign) return false;
	if(a._digits.size() != b._digits.size()) return false;

	auto ia = a._digits.begin();
	auto ib = b._digits.begin();

	for (; ia != a._digits.end() && ib != b._digits.end(); ia++, ib++) {
		if (*ia > *ib) return false;
	}

	return true;
}

bool operator>(const BigNum& a, const BigNum& b) {
	if (a.sign == Sign::NEGATIVE && b.sign == Sign::POSITIVE) return false;
	if (a.sign == Sign::POSITIVE && b.sign == Sign::NEGATIVE) return true;

	if (a._digits.size() > b._digits.size()) return true;
	if (a._digits.size() < b._digits.size()) return false;

	auto ia = a._digits.begin();
	auto ib = b._digits.begin();

	for (; ia != a._digits.end() && ib != b._digits.end(); ia++, ib++) {
		if (*ia > *ib) return false;
	}

	return true;
}

bool operator>=(const BigNum& a, const BigNum& b) {
	return a == b || a > b;
}

bool operator<(const BigNum& a, const BigNum b) {
	return !(a == b) && !(a > b);
}

BigNum operator+=(BigNum& a, const BigNum& b) {
	BigNum c;
	c._digits = b._digits;
	c.sign = b.sign;

	if (a.sign != b.sign) {
		if(!(a >= b)) {
			swap_digits(a, c);
			swap_sign(a, c);
		}

		_subtract(a, c);
		normalize(a);
		return a;
	}

	_add(a, c);
	normalize(a);
	return a;
}


BigNum operator+(const BigNum& a, const BigNum& b) {
	BigNum a1;
	a1._digits = a._digits;
	a1.sign = a.sign;

	BigNum b1;
	b1._digits = b._digits;
	b1.sign = b.sign;

	if (a1.sign != b1.sign) {
		if(!(a1 >= b1)) {
			swap_digits(a1, b1);
			swap_sign(a1, b1);
		}

		_subtract(a1, b1);
		normalize(a1);
		return a1;
	}

	_add(a1, b1);
	normalize(a1);
	return a1;
}

BigNum operator-=(BigNum& a, const BigNum& b) {
	BigNum c;
	c._digits = b._digits;
	c.sign = b.sign;

	if(a < b) {
		swap_digits(a, c);
		swap_sign(a, c);
	}

	if(a.sign == Sign::POSITIVE && c.sign == Sign::POSITIVE) {
		_subtract(a, c);
		return a;
	}

	_add(a, c);

	return a;
}

BigNum operator-(const BigNum& a, const BigNum& b) {
	BigNum a1;
	a1._digits = a._digits;
	a1.sign = a.sign;

	BigNum b1;
	b1._digits = b._digits;
	b1.sign = b.sign;

	if(a < b) {
		swap_digits(a1, b1);
		swap_sign(a1, b1);
	}

	if(a1.sign == Sign::POSITIVE && b1.sign == Sign::POSITIVE) {
		_subtract(a1, b1);
		return a1;
	}

	_add(a1, b1);

	return a1;
}

template<typename PrimitiveType>
BigNum operator*=(BigNum& a, PrimitiveType b) {
	_multiply_by_int(a, b);
	return a;
}

int main() {
	BigNum a = fromString("1");
	int b;

	std::cin >> b;

	// factorial test
	while(b) {
		a *= b;
		b--;
	}

	std::cout << a << std::endl;
}
