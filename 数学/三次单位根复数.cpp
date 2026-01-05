// x^3 = 1, x != 1
template<class Int>
struct Complex3 {
	Int a, b;
	Complex3(Int a = Int(), Int b = Int()) : a(a), b(b) {}

	Complex3 operator+(const Complex3& rhs) const {
		return {a + rhs.a, b + rhs.b};
	}

	Complex3 operator-(const Complex3& rhs) const {
		return {a - rhs.a, b - rhs.b};
	}

	Complex3 operator*(const Complex3& rhs) const {
		Int ac = a * rhs.a;
		Int bd = b * rhs.b;
		Int ad = a * rhs.b;
		Int bc = b * rhs.a;

		return {ac - bd, ad + bc - bd};
	}

	Complex3 operator/(const Complex3& rhs) const {
		return (*this) * rhs.inv();
	}

	Complex3 inv() const {
		Int norm = a * a - a * b + b * b;
		return Complex3<Int>((a - b) / norm,  - b / norm);
	}

	Complex3& operator+=(const Complex3& rhs) {
		*this = (*this) + rhs;
		return *this;
	}

	Complex3& operator-=(const Complex3& rhs) {
		*this = (*this) - rhs;
		return *this;
	}

	Complex3& operator*=(const Complex3& rhs) {
		*this = (*this) * rhs;
		return *this;
	}

	Complex3& operator/=(const Complex3& rhs) {
		*this = (*this) / rhs;
		return *this;
	}
	bool isZero() const {
		return a == 0 && b == 0;
	}
};
