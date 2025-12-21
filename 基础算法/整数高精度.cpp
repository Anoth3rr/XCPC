template<class T>
constexpr T ksm(T a, ll b) {
    T res = 1;
    while (b) {
        if (b & 1) res *= a;
        b >>= 1;
        a *= a;
    }
    return res;
}

struct BigInt {
    static const uint32_t BASE = 1000000000u;
    static const int BASE_DIGS = 9;
    vector<uint32_t> d;
    int sign;
    BigInt(): sign(0) {}
    BigInt(long long v) { *this = v; }
    BigInt& operator=(long long v) {
        d.clear();
        if (v == 0) { sign = 0; return *this; }
        sign = v < 0 ? -1 : 1;
        unsigned long long av = (v < 0) ? -v : v;
        while (av) {
            d.push_back((uint32_t)(av % BASE));
            av /= BASE;
        }
        return *this;
    }
    BigInt(const string &s) { read(s); }

    bool isZero() const { return sign == 0; }
    void normalize() {
        while (!d.empty() && d.back() == 0) d.pop_back();
        if (d.empty()) sign = 0;
    }

    string toString() const {
        if (isZero()) return "0";
        stringstream ss;
        if (sign < 0) ss << '-';
        ss << (d.empty() ? 0 : d.back());
        char buf[32];
        for (int i = (int)d.size() - 2; i >= 0; --i) {
            snprintf(buf, sizeof(buf), "%09u", d[i]);
            ss << buf;
        }
        return ss.str();
    }

    void read(const string &s) {
        d.clear();
        sign = 1;
        int pos = 0;
        while (pos < (int)s.size() && isspace((unsigned char)s[pos])) ++pos;
        if (pos < (int)s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-') sign = -1;
            ++pos;
        }
        while (pos < (int)s.size() && s[pos] == '0') ++pos;
        if (pos == (int)s.size()) { sign = 0; return; }
        for (int i = (int)s.size() - 1; i >= pos; i -= BASE_DIGS) {
            int x = 0;
            int l = max(pos, i - BASE_DIGS + 1);
            for (int j = l; j <= i; ++j) x = x * 10 + (s[j] - '0');
            d.push_back((uint32_t)x);
        }
        normalize();
    }

    static int cmpAbs(const BigInt &a, const BigInt &b) {
        if (a.d.size() != b.d.size()) return a.d.size() < b.d.size() ? -1 : 1;
        for (int i = (int)a.d.size() - 1; i >= 0; --i)
            if (a.d[i] != b.d[i]) return a.d[i] < b.d[i] ? -1 : 1;
        return 0;
    }
    static int cmp(const BigInt &a, const BigInt &b) {
        if (a.sign != b.sign) return a.sign < b.sign ? -1 : 1;
        if (a.sign == 0) return 0;
        int c = cmpAbs(a, b);
        return a.sign > 0 ? c : -c;
    }

    static BigInt addAbs(const BigInt &a, const BigInt &b) {
        BigInt res;
        uint64_t carry = 0;
        size_t n = max(a.d.size(), b.d.size());
        res.d.assign(n, 0);
        for (size_t i = 0; i < n; ++i) {
            uint64_t av = i < a.d.size() ? a.d[i] : 0;
            uint64_t bv = i < b.d.size() ? b.d[i] : 0;
            uint64_t sum = av + bv + carry;
            res.d[i] = (uint32_t)(sum % BASE);
            carry = sum / BASE;
        }
        if (carry) res.d.push_back((uint32_t)carry);
        res.sign = res.d.empty() ? 0 : 1;
        return res;
    }

    static BigInt subAbs(const BigInt &a, const BigInt &b) {
        BigInt res;
        res.d.assign(a.d.size(), 0);
        int64_t carry = 0;
        for (size_t i = 0; i < a.d.size(); ++i) {
            int64_t av = a.d[i];
            int64_t bv = i < b.d.size() ? b.d[i] : 0;
            int64_t cur = av - bv - carry;
            if (cur < 0) {
                cur += BASE;
                carry = 1;
            } else carry = 0;
            res.d[i] = (uint32_t)cur;
        }
        res.normalize();
        res.sign = res.d.empty() ? 0 : 1;
        return res;
    }

    BigInt operator+(const BigInt &v) const {
        if (sign == 0) return v;
        if (v.sign == 0) return *this;
        BigInt res;
        if (sign == v.sign) {
            res = addAbs(*this, v);
            res.sign = sign;
        } else {
            int cmpab = cmpAbs(*this, v);
            if (cmpab >= 0) {
                res = subAbs(*this, v);
                res.sign = sign;
            } else {
                res = subAbs(v, *this);
                res.sign = v.sign;
            }
        }
        if (res.isZero()) res.sign = 0;
        return res;
    }

    BigInt operator-(const BigInt &v) const {
        if (v.isZero()) return *this;
        BigInt tmp = v;
        tmp.sign = -tmp.sign;
        return *this + tmp;
    }

    static BigInt mulNaive(const BigInt &a, const BigInt &b) {
        if (a.isZero() || b.isZero()) return BigInt(0);
        BigInt res;
        size_t n = a.d.size(), m = b.d.size();
        res.d.assign(n + m, 0);
        for (size_t i = 0; i < n; ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < m || carry; ++j) {
                uint64_t cur = res.d[i + j] + carry + uint64_t(a.d[i]) * (j < m ? b.d[j] : 0);
                res.d[i + j] = (uint32_t)(cur % BASE);
                carry = cur / BASE;
            }
        }
        res.sign = a.sign * b.sign;
        res.normalize();
        return res;
    }

    static BigInt mulInt(const BigInt &a, long long m) {
        if (a.isZero() || m == 0) return BigInt(0);
        BigInt res;
        long long sign2 = m < 0 ? -1 : 1;
        unsigned long long mm = (m < 0 ? -m : m);
        res.d.assign(a.d.size(), 0);
        unsigned long long carry = 0;
        for (size_t i = 0; i < a.d.size() || carry; ++i) {
            unsigned long long cur = carry + (unsigned long long)(i < a.d.size() ? a.d[i] : 0) * mm;
            if (i >= res.d.size()) res.d.push_back(0);
            res.d[i] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        res.sign = a.sign * sign2;
        res.normalize();
        return res;
    }

private:
    struct Complex { double re, im; Complex(double r = 0, double i = 0): re(r), im(i) {} Complex operator+(const Complex&o)const {return Complex(re + o.re, im + o.im);} Complex operator-(const Complex&o)const {return Complex(re - o.re, im - o.im);} Complex operator*(const Complex&o)const {return Complex(re * o.re - im * o.im, re * o.im + im * o.re);} };

    static void fft(vector<Complex> &a, bool invert) {
        int n = (int)a.size();
        static vector<int> rev;
        static vector<Complex> roots{Complex(0, 0), Complex(1, 0)};
        if ((int)rev.size() != n) {
            int k = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; ++i)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }
        for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
        if ((int)roots.size() < n) {
            int k = __builtin_ctz(roots.size());
            roots.resize(n);
            while ((1 << k) < n) {
                double angle = acos(-1.0) / (1 << k);
                for (int i = 1 << (k - 1); i < (1 << k); ++i) {
                    double ang = angle * (2 * i + 1 - (1 << k));
                    roots[2 * i] = roots[i];
                    roots[2 * i + 1] = Complex(cos(ang), sin(ang));
                }
                ++k;
            }
        }
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; ++j) {
                    Complex u = a[i + j];
                    Complex v = a[i + j + len] * roots[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }
        if (invert) {
            reverse(a.begin() + 1, a.end());
            for (int i = 0; i < n; ++i) {
                a[i].re /= n;
                a[i].im /= n;
            }
        }
    }

    static BigInt mulFFT(const BigInt &a, const BigInt &b) {
        if (a.isZero() || b.isZero()) return BigInt(0);
        const int baseFFT = 1000;
        const int baseFDigs = 3;
        vector<int> fa, fb;
        for (size_t i = 0; i < a.d.size(); ++i) {
            uint32_t x = a.d[i];
            for (int k = 0; k < BASE_DIGS / baseFDigs; ++k) {
                fa.push_back(x % baseFFT);
                x /= baseFFT;
            }
        }
        while (!fa.empty() && fa.back() == 0) fa.pop_back();
        for (size_t i = 0; i < b.d.size(); ++i) {
            uint32_t x = b.d[i];
            for (int k = 0; k < BASE_DIGS / baseFDigs; ++k) {
                fb.push_back(x % baseFFT);
                x /= baseFFT;
            }
        }
        while (!fb.empty() && fb.back() == 0) fb.pop_back();

        int n = 1;
        while (n < (int)fa.size() + (int)fb.size()) n <<= 1;
        vector<Complex> A(n), B(n);
        for (size_t i = 0; i < fa.size(); ++i) A[i].re = fa[i];
        for (size_t i = 0; i < fb.size(); ++i) B[i].re = fb[i];

        fft(A, false); fft(B, false);
        for (int i = 0; i < n; ++i) A[i] = A[i] * B[i];
        fft(A, true);

        vector<uint64_t> resv(n);
        for (int i = 0; i < n; ++i) resv[i] = (uint64_t)(A[i].re + 0.5);
        BigInt res;
        const uint64_t perLimb = 1000000000ULL;
        uint64_t curVal = 0;
        uint64_t mult = 1;
        for (size_t i = 0; i < resv.size(); ++i) {
            uint64_t val = resv[i];
            curVal += val * mult;
            mult *= baseFFT;
            if (mult >= perLimb) {
                res.d.push_back((uint32_t)(curVal % perLimb));
                curVal /= perLimb;
                mult %= perLimb;
            }
        }
        while (curVal > 0) {
            res.d.push_back((uint32_t)(curVal % perLimb));
            curVal /= perLimb;
        }
        res.sign = a.sign * b.sign;
        res.normalize();
        return res;
    }

public:
    static const size_t FFT_MUL_THRESHOLD = 1500;
    BigInt operator*(const BigInt &v) const {
        if (isZero() || v.isZero()) return BigInt(0);
        size_t n = d.size(), m = v.d.size();
        if (min(n, m) == 0) return BigInt(0);
        if (n + m < FFT_MUL_THRESHOLD) return mulNaive(*this, v);
        else return mulFFT(*this, v);
    }

    static pair<BigInt, long long> divmodInt(const BigInt &a, long long b) {
        if (b == 0) throw runtime_error("div by zero");
        BigInt q;
        q.d.assign(a.d.size(), 0);
        unsigned long long carry = 0;
        unsigned long long ub = (b < 0) ? -b : b;
        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            unsigned long long cur = a.d[i] + carry * BASE;
            q.d[i] = (uint32_t)(cur / ub);
            carry = cur % ub;
        }
        q.sign = (q.d.empty() ? 0 : (a.sign * (b < 0 ? -1 : 1)));
        q.normalize();
        long long rem = (long long)carry;
        if (a.sign < 0) rem = -rem;
        return {q, rem};
    }

    static pair<BigInt, BigInt> divmod(const BigInt &a, const BigInt &b) {
        if (b.isZero()) throw runtime_error("div by zero");
        if (a.isZero()) return {BigInt(0), BigInt(0)};
        int signRes = a.sign * b.sign;
        BigInt aa = a; aa.sign = 1;
        BigInt bb = b; bb.sign = 1;
        if (cmpAbs(aa, bb) < 0) return {BigInt(0), a};

        size_t n = aa.d.size();
        size_t m = bb.d.size();
        uint64_t f = (uint64_t)BASE / ((uint64_t)bb.d.back() + 1);
        aa = mulInt(aa, f);
        bb = mulInt(bb, f);
        aa.d.push_back(0);
        BigInt q;
        q.d.assign(n - m + 1, 0);
        for (int i = (int)(n - m); i >= 0; --i) {
            uint64_t ai_m = (uint64_t)aa.d[i + m] * BASE + aa.d[i + m - 1];
            uint64_t qt = ai_m / bb.d[m - 1];
            if (qt >= BASE) qt = BASE - 1;
            uint64_t carry = 0, borrow = 0;
            for (size_t j = 0; j < m; ++j) {
                unsigned long long prod = qt * (unsigned long long)bb.d[j] + carry;
                carry = prod / BASE;
                unsigned long long sub = aa.d[i + j] - (prod % BASE) - borrow;
                if ((long long)sub < 0) { sub += BASE; borrow = 1; } else borrow = 0;
                aa.d[i + j] = (uint32_t)sub;
            }
            unsigned long long sub = aa.d[i + m] - carry - borrow;
            if ((long long)sub < 0) {
                --qt;
                unsigned long long add = 0;
                for (size_t j = 0; j < m; ++j) {
                    unsigned long long cur = (unsigned long long)aa.d[i + j] + bb.d[j] + add;
                    aa.d[i + j] = (uint32_t)(cur % BASE);
                    add = cur / BASE;
                }
                aa.d[i + m] = (uint32_t)((unsigned long long)aa.d[i + m] + add);
            } else {
                aa.d[i + m] = (uint32_t)sub;
            }
            q.d[i] = (uint32_t)qt;
        }
        q.sign = signRes;
        q.normalize();
        BigInt r;
        r.d.assign(aa.d.begin(), aa.d.begin() + (size_t)m);
        auto tmp = divmodInt(r, (long long)f);
        r = tmp.first;
        r.sign = a.sign;
        r.normalize();
        return {q, r};
    }

    static BigInt mod(const BigInt &a, const BigInt &b) {
        return divmod(a, b).second;
    }

    static BigInt divBig(const BigInt &a, const BigInt &b) {
        return divmod(a, b).first;
    }

    BigInt operator/(const BigInt &v) const { return divBig(*this, v); }
    BigInt operator%(const BigInt &v) const { return mod(*this, v); }
    BigInt operator/(long long v) const { return divmodInt(*this, v).first; }
    BigInt operator%(long long v) const { return BigInt(divmodInt(*this, v).second); }

    static BigInt pow(BigInt a, unsigned long long e) {
        BigInt res(1);
        while (e) {
            if (e & 1) res = res * a;
            a = a * a;
            e >>= 1;
        }
        return res;
    }

    static BigInt mulSmall(const BigInt &a, uint32_t small) { return mulInt(a, small); }

    friend ostream& operator<<(ostream &os, const BigInt &x) { os << x.toString(); return os; }
    friend istream& operator>>(istream &is, BigInt &x) { string s; is >> s; x.read(s); return is; }

    BigInt& operator+=(const BigInt &v) { *this = *this + v; return *this; }
    BigInt& operator-=(const BigInt &v) { *this = *this - v; return *this; }
    BigInt& operator*=(const BigInt &v) { *this = *this * v; return *this; }
    BigInt& operator/=(const BigInt &v) { *this = *this / v; return *this; }

    friend bool operator==(const BigInt &a, const BigInt &b) { return cmp(a, b) == 0; }
    friend bool operator!=(const BigInt &a, const BigInt &b) { return cmp(a, b) != 0; }
    friend bool operator<(const BigInt &a, const BigInt &b) { return cmp(a, b) < 0; }
    friend bool operator<=(const BigInt &a, const BigInt &b) { return cmp(a, b) <= 0; }
    friend bool operator>(const BigInt &a, const BigInt &b) { return cmp(a, b) > 0; }
    friend bool operator>=(const BigInt &a, const BigInt &b) { return cmp(a, b) >= 0; }
};