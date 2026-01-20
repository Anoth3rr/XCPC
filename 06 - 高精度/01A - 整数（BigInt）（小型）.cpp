class BigInt {
  public:
    vector<int> d;
    int sign;

    BigInt(long long x = 0) { *this = x; }

    BigInt &operator=(long long x) {
        d.clear();
        if (x == 0)
            return sign = 0, *this;
        sign = x < 0 ? -1 : 1;
        x = llabs(x);
        while (x)
            d.push_back(x % 10), x /= 10;
        return *this;
    }

    BigInt(const string &s) { *this = s; }

    BigInt &operator=(const string &s) {
        d.clear();
        sign = 1;
        int i = 0;
        if (s[0] == '-')
            sign = -1, i = 1;
        for (int j = s.size() - 1; j >= i; j--)
            d.push_back(s[j] - '0');
        normalize();
        return *this;
    }

    void normalize() {
        while (!d.empty() && d.back() == 0)
            d.pop_back();
        if (d.empty())
            sign = 0;
    }

    friend ostream &operator<<(ostream &os, const BigInt &x) {
        if (x.sign <= 0 && x.sign != 0)
            os << '-';
        if (x.sign == 0)
            return os << '0';
        for (int i = x.d.size() - 1; i >= 0; i--)
            os << x.d[i];
        return os;
    }

    static int cmpAbs(const BigInt &a, const BigInt &b) {
        if (a.d.size() != b.d.size())
            return a.d.size() < b.d.size() ? -1 : 1;
        for (int i = a.d.size() - 1; i >= 0; i--)
            if (a.d[i] != b.d[i])
                return a.d[i] < b.d[i] ? -1 : 1;
        return 0;
    }

    static BigInt addAbs(const BigInt &a, const BigInt &b) {
        BigInt c;
        int carry = 0;
        for (int i = 0; i < a.d.size() || i < b.d.size() || carry; i++) {
            int x = carry;
            if (i < a.d.size())
                x += a.d[i];
            if (i < b.d.size())
                x += b.d[i];
            c.d.push_back(x % 10);
            carry = x / 10;
        }
        c.sign = 1;
        return c;
    }

    static BigInt subAbs(const BigInt &a, const BigInt &b) {
        BigInt c;
        int carry = 0;
        for (int i = 0; i < a.d.size(); i++) {
            int x = a.d[i] - carry - (i < b.d.size() ? b.d[i] : 0);
            if (x < 0)
                x += 10, carry = 1;
            else
                carry = 0;
            c.d.push_back(x);
        }
        c.sign = 1;
        c.normalize();
        return c;
    }

    friend BigInt operator+(const BigInt &a, const BigInt &b) {
        if (a.sign == 0)
            return b;
        if (b.sign == 0)
            return a;
        if (a.sign == b.sign) {
            BigInt c = addAbs(a, b);
            c.sign = a.sign;
            return c;
        }
        int cmp = cmpAbs(a, b);
        if (cmp == 0)
            return BigInt(0);
        if (cmp > 0) {
            BigInt c = subAbs(a, b);
            c.sign = a.sign;
            return c;
        }
        BigInt c = subAbs(b, a);
        c.sign = b.sign;
        return c;
    }

    friend BigInt operator-(const BigInt &a, const BigInt &b) {
        BigInt t = b;
        t.sign = -t.sign;
        return a + t;
    }

    friend BigInt operator*(const BigInt &a, const BigInt &b) {
        if (a.sign == 0 || b.sign == 0)
            return BigInt(0);
        BigInt c;
        c.d.assign(a.d.size() + b.d.size(), 0);
        for (int i = 0; i < a.d.size(); i++)
            for (int j = 0; j < b.d.size(); j++)
                c.d[i + j] += a.d[i] * b.d[j];
        for (int i = 0; i < c.d.size() - 1; i++)
            c.d[i + 1] += c.d[i] / 10, c.d[i] %= 10;
        c.sign = a.sign * b.sign;
        c.normalize();
        return c;
    }
    BigInt &operator+=(const BigInt &v) {
        *this = *this + v;
        return *this;
    }
    BigInt &operator-=(const BigInt &v) {
        *this = *this - v;
        return *this;
    }
    BigInt &operator*=(const BigInt &v) {
        *this = *this * v;
        return *this;
    }
    BigInt &operator/=(const BigInt &v) {
        *this = *this / v;
        return *this;
    }
};
