template<typename T>
class Complex {
public:
	T a, b;

	constexpr Complex(T r = 0, T i = 0) : a(r), b(i) {}

	constexpr Complex operator+(const Complex& rhs) const {
		return {a + rhs.a, b + rhs.b};
	}

	constexpr Complex operator-(const Complex& rhs) const {
		return {a - rhs.a, b - rhs.b};
	}

	constexpr Complex operator*(const Complex& rhs) const {
		return {
			a * rhs.a - b * rhs.b,
			a * rhs.b + b * rhs.a
		};
	}

	Complex operator/(const Complex& rhs) const {
		T denom = rhs.a * rhs.a + rhs.b * rhs.b;
		assert(denom != 0);
		return {
			(a * rhs.a + b * rhs.b) / denom,
			(b * rhs.a - a * rhs.b) / denom
		};
	}

	Complex inv() const {
		T denom = a * a + b * b;
		return Complex(a / denom, -b / denom);
	}

	Complex& operator+=(const Complex& rhs) {
		return (*this) + rhs;
	}

	Complex& operator-=(const Complex& rhs) {
		return (*this) - rhs;
	}

	Complex& operator*=(const Complex& rhs) {
		return (*this) * rhs;
	}

	Complex& operator/=(const Complex& rhs) {
		return (*this) / rhs;
	}

	T norm() const {
		return a * a + b * b;
	}

	bool operator==(const Complex& rhs) const {
		return a == rhs.a && b == rhs.b;
	}

	bool operator!=(const Complex& rhs) const {
		return !(*this == rhs);
	}

	bool operator<(const Complex& rhs) const {
		return norm() < rhs.norm();
	}

	bool operator>(const Complex& rhs) const {
		return norm() > rhs.norm();
	}

	friend ostream& operator<<(ostream& os, const Complex& c) {
		os << "(" << c.a;
		if (c.b >= 0) os << "+";
		os << c.b << "i)";
		return os;
	}
};