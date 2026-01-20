template <class T>
constexpr T ksm(T a, ll b) {
    T res = 1;
    while (b) {
        if (b & 1)
            res *= a;
        b >>= 1;
        a *= a;
    }
    return res;
}

struct BigInt {
  public:
    using ui = unsigned;
    using uc = unsigned char;
    using ll = long long;
    using ull = unsigned long long;
    static constexpr ui BASE = 1000000000u;
    static constexpr int DIGS = 9;

  private:
    using cd = Complex<double>;
    static constexpr double PI = acos(-1);

    template <typename T>
    class Complex {
      public:
        T a, b;

        constexpr Complex(T r = 0, T i = 0) : a(r), b(i) {}

        constexpr Complex operator+(const Complex &rhs) const {
            return {a + rhs.a, b + rhs.b};
        }

        constexpr Complex operator-(const Complex &rhs) const {
            return {a - rhs.a, b - rhs.b};
        }

        constexpr Complex operator*(const Complex &rhs) const {
            return {
                a * rhs.a - b * rhs.b,
                a * rhs.b + b * rhs.a};
        }
    };

    void fft(vector<cd> &a, bool invert) {
        int n = a.size();
        static vector<int> rev;
        static vector<cd> roots{{0, 0}, {1, 0}};

        if (rev.size() != n) {
            int k = __builtin_ctz(n);
            rev.assign(n, 0);
            for (int i = 0; i < n; i++)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }

        if (roots.size() < n) {
            int k = __builtin_ctz(roots.size());
            roots.resize(n);
            while ((1 << k) < n) {
                double angle = 2 * PI / (1 << (k + 1));
                for (int i = 1 << (k - 1); i < (1 << k); i++) {
                    roots[i << 1] = roots[i];
                    double ang = angle * (2 * i + 1 - (1 << k));
                    roots[i << 1 | 1] = cd(cos(ang), sin(ang));
                }
                k++;
            }
        }

        for (int i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);

        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += len << 1) {
                for (int j = 0; j < len; j++) {
                    cd u = a[i + j];
                    cd v = a[i + j + len] * roots[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }

        if (invert) {
            reverse(a.begin() + 1, a.end());
            for (cd &x : a)
                x /= n;
        }
    }

    static BigInt mulFFT(const BigInt &a, const BigInt &b) {
        if (a.isZero() || b.isZero())
            return BigInt(0);
        constexpr int FBASE = 1000;
        constexpr int FDIGS = 3;
        vector<int> fa, fb;
        for (int i = 0; i < a.d.size(); ++i) {
            ui x = a.d[i];
            for (int k = 0; k < DIGS / FDIGS; ++k) {
                fa.push_back(x % FBASE);
                x /= FBASE;
            }
        }
        while (!fa.empty() && fa.back() == 0)
            fa.pop_back();
        for (int i = 0; i < b.d.size(); ++i) {
            ui x = b.d[i];
            for (int k = 0; k < DIGS / FDIGS; ++k) {
                fb.push_back(x % FBASE);
                x /= FBASE;
            }
        }
        while (!fb.empty() && fb.back() == 0)
            fb.pop_back();

        int n = 1;
        while (n < fa.size() + fb.size())
            n <<= 1;
        vector<Complex> A(n), B(n);
        for (int i = 0; i < fa.size(); ++i)
            A[i].a = fa[i];
        for (int i = 0; i < fb.size(); ++i)
            B[i].a = fb[i];

        fft(A, false);
        fft(B, false);
        for (int i = 0; i < n; ++i)
            A[i] = A[i] * B[i];
        fft(A, true);

        vector<ull> resv(n);
        for (int i = 0; i < n; ++i)
            resv[i] = (ull)(A[i].a + 0.5);
        BigInt res;
        const ull perLimb = 1000000000ull;
        ull curVal = 0;
        ull mult = 1;
        for (int i = 0; i < resv.size(); ++i) {
            ull val = resv[i];
            curVal += val * mult;
            mult *= FBASE;
            if (mult >= perLimb) {
                res.d.push_back((ui)(curVal % perLimb));
                curVal /= perLimb;
                mult %= perLimb;
            }
        }
        while (curVal > 0) {
            res.d.push_back((ui)(curVal % perLimb));
            curVal /= perLimb;
        }
        res.sign = a.sign * b.sign;
        res.normalize();
        return res;
    }

    vector<ui> d;
    short sign;

  public:
    BigInt() : sign(0) {}
    BigInt(ll v) { *this = v; }
    BigInt(const string &s) { read(s); }
    BigInt &operator=(ll v) {
        d.clear();
        if (v == 0) {
            sign = 0;
            return *this;
        }
        sign = v < 0 ? -1 : 1;
        ull av = (v < 0) ? -v : v;
        while (av) {
            d.push_back((ui)(av % BASE));
            av /= BASE;
        }
        return *this;
    }

    bool isZero() const { return sign == 0; }

    void normalize() {
        while (!d.empty() && d.back() == 0)
            d.pop_back();
        if (d.empty())
            sign = 0;
    }

    string to_string() const {
        if (isZero())
            return "0";
        stringstream ss;
        if (sign < 0)
            ss << '-';
        ss << (d.empty() ? 0 : d.back());
        char buf[32];
        for (int i = d.size() - 2; i >= 0; --i) {
            snprintf(buf, sizeof(buf), "%09u", d[i]);
            ss << buf;
        }
        return ss.str();
    }

    void read(const string &s) {
        d.clear();
        sign = 1;
        int pos = 0;
        while (pos < s.size() && !isdigit(s[pos])) {
            if (s[pos] == '-')
                sign = -1;
            ++pos;
        }
        if (pos == s.size()) {
            sign = 0;
            return;
        }
        while (pos < s.size() && s[pos] == '0')
            ++pos;

        if (pos == s.size()) {
            sign = 0;
            return;
        }

        for (int i = s.size() - 1; i >= pos; i -= DIGS) {
            ui x = 0;
            int l = max(pos, i - DIGS + 1);
            for (int j = l; j <= i; ++j)
                x = x * 10 + (s[j] - '0');
            d.push_back(x);
        }

        normalize();
    }

  private:
    static int cmpAbs(const BigInt &a, const BigInt &b) {
        if (a.d.size() != b.d.size())
            return a.d.size() < b.d.size() ? -1 : 1;
        for (int i = a.d.size() - 1; i >= 0; --i)
            if (a.d[i] != b.d[i])
                return a.d[i] < b.d[i] ? -1 : 1;
        return 0;
    }
    static int cmp(const BigInt &a, const BigInt &b) {
        if (a.sign != b.sign)
            return a.sign < b.sign ? -1 : 1;
        if (a.sign == 0)
            return 0;
        int c = cmpAbs(a, b);
        return a.sign > 0 ? c : -c;
    }

    static BigInt addAbs(const BigInt &a, const BigInt &b) {
        BigInt res;
        ull carry = 0;
        int n = max(a.d.size(), b.d.size());
        res.d.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            ull av = i < a.d.size() ? a.d[i] : 0;
            ull bv = i < b.d.size() ? b.d[i] : 0;
            ull sum = av + bv + carry;
            res.d[i] = (ui)(sum % BASE);
            carry = sum / BASE;
        }
        if (carry)
            res.d.push_back((ui)carry);
        res.sign = res.d.empty() ? 0 : 1;
        return res;
    }

    static BigInt subAbs(const BigInt &a, const BigInt &b) {
        BigInt res;
        res.d.assign(a.d.size(), 0);
        ull carry = 0;
        for (int i = 0; i < a.d.size(); ++i) {
            ull av = a.d[i];
            ull bv = i < b.d.size() ? b.d[i] : 0;
            ull cur = av - bv - carry;
            if (cur < 0) {
                cur += BASE;
                carry = 1;
            } else
                carry = 0;
            res.d[i] = (ui)cur;
        }
        res.normalize();
        res.sign = res.d.empty() ? 0 : 1;
        return res;
    }

  public:
    BigInt operator+(const BigInt &v) const {
        if (sign == 0)
            return v;
        if (v.sign == 0)
            return *this;
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
        if (res.isZero())
            res.sign = 0;
        return res;
    }

    BigInt operator-(const BigInt &v) const {
        if (v.isZero())
            return *this;
        BigInt tmp = v;
        tmp.sign = -tmp.sign;
        return *this + tmp;
    }

    static BigInt mulNaive(const BigInt &a, const BigInt &b) {
        if (a.isZero() || b.isZero())
            return BigInt(0);
        BigInt res;
        int n = a.d.size(), m = b.d.size();
        res.d.assign(n + m, 0);
        for (int i = 0; i < n; ++i) {
            ull carry = 0;
            for (int j = 0; j < m || carry; ++j) {
                ull cur = res.d[i + j] + carry + ull(a.d[i]) * (j < m ? b.d[j] : 0);
                res.d[i + j] = (ui)(cur % BASE);
                carry = cur / BASE;
            }
        }
        res.sign = a.sign * b.sign;
        res.normalize();
        return res;
    }

    static BigInt mulInt(const BigInt &a, ll m) {
        if (a.isZero() || m == 0)
            return BigInt(0);
        BigInt res;
        short sign2 = m < 0 ? -1 : 1;
        ull mm = (m < 0 ? -m : m);
        res.d.assign(a.d.size(), 0);
        unsigned long long carry = 0;
        for (int i = 0; i < a.d.size() || carry; ++i) {
            unsigned long long cur = carry + (unsigned long long)(i < a.d.size() ? a.d[i] : 0) * mm;
            if (i >= res.d.size())
                res.d.push_back(0);
            res.d[i] = (ui)(cur % BASE);
            carry = cur / BASE;
        }
        res.sign = a.sign * sign2;
        res.normalize();
        return res;
    }

  public:
    static const int FFT_MUL_THRESHOLD = 1500;
    BigInt operator*(const BigInt &v) const {
        if (isZero() || v.isZero())
            return BigInt(0);
        int n = d.size(), m = v.d.size();
        if (min(n, m) == 0)
            return BigInt(0);
        if (n + m < FFT_MUL_THRESHOLD)
            return mulNaive(*this, v);
        else
            return mulFFT(*this, v);
    }

    static pair<BigInt, ll> divmodInt(const BigInt &a, long long b) {
        if (b == 0)
            throw runtime_error("div by zero");
        BigInt q;
        q.d.assign(a.d.size(), 0);
        ull carry = 0;
        ull ub = (b < 0) ? -b : b;
        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            ull cur = a.d[i] + carry * BASE;
            q.d[i] = (ui)(cur / ub);
            carry = cur % ub;
        }
        q.sign = (q.d.empty() ? 0 : (a.sign * (b < 0 ? -1 : 1)));
        q.normalize();
        ll rem = (ll)carry;
        if (a.sign < 0)
            rem = -rem;
        return {q, rem};
    }

    static pair<BigInt, BigInt> divmod(const BigInt &a, const BigInt &b) {
        if (b.isZero())
            throw runtime_error("div by zero");
        if (a.isZero())
            return {BigInt(0), BigInt(0)};
        int signRes = a.sign * b.sign;
        BigInt aa = a;
        aa.sign = 1;
        BigInt bb = b;
        bb.sign = 1;
        if (cmpAbs(aa, bb) < 0)
            return {BigInt(0), a};

        int n = aa.d.size();
        int m = bb.d.size();
        ull f = (ull)BASE / ((ull)bb.d.back() + 1);
        aa = mulInt(aa, f);
        bb = mulInt(bb, f);
        aa.d.push_back(0);
        BigInt q;
        q.d.assign(n - m + 1, 0);
        for (int i = (int)(n - m); i >= 0; --i) {
            ull ai_m = (ull)aa.d[i + m] * BASE + aa.d[i + m - 1];
            ull qt = ai_m / bb.d[m - 1];
            if (qt >= BASE)
                qt = BASE - 1;
            ull carry = 0, borrow = 0;
            for (int j = 0; j < m; ++j) {
                ull prod = qt * (ull)bb.d[j] + carry;
                carry = prod / BASE;
                ull sub = aa.d[i + j] - (prod % BASE) - borrow;
                if ((ll)sub < 0) {
                    sub += BASE;
                    borrow = 1;
                } else
                    borrow = 0;
                aa.d[i + j] = (ui)sub;
            }
            ull sub = aa.d[i + m] - carry - borrow;
            if ((ll)sub < 0) {
                --qt;
                ull add = 0;
                for (int j = 0; j < m; ++j) {
                    ull cur = (ull)aa.d[i + j] + bb.d[j] + add;
                    aa.d[i + j] = (ui)(cur % BASE);
                    add = cur / BASE;
                }
                aa.d[i + m] = (ui)((ull)aa.d[i + m] + add);
            } else {
                aa.d[i + m] = (ui)sub;
            }
            q.d[i] = (ui)qt;
        }
        q.sign = signRes;
        q.normalize();
        BigInt r;
        r.d.assign(aa.d.begin(), aa.d.begin() + (int)m);
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
            if (e & 1)
                res = res * a;
            a = a * a;
            e >>= 1;
        }
        return res;
    }

    static BigInt mulSmall(const BigInt &a, ui small) { return mulInt(a, small); }

    friend ostream &operator<<(ostream &os, const BigInt &x) {
        os << x.to_string();
        return os;
    }
    friend istream &operator>>(istream &is, BigInt &x) {
        string s;
        is >> s;
        x.aad(s);
        return is;
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

    friend bool operator==(const BigInt &a, const BigInt &b) { return cmp(a, b) == 0; }
    friend bool operator!=(const BigInt &a, const BigInt &b) { return cmp(a, b) != 0; }
    friend bool operator<(const BigInt &a, const BigInt &b) { return cmp(a, b) < 0; }
    friend bool operator<=(const BigInt &a, const BigInt &b) { return cmp(a, b) <= 0; }
    friend bool operator>(const BigInt &a, const BigInt &b) { return cmp(a, b) > 0; }
    friend bool operator>=(const BigInt &a, const BigInt &b) { return cmp(a, b) >= 0; }
};