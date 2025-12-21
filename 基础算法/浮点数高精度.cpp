class Decimal {
public:
    static void setPrecision(int digits) {
        if (digits < 0) digits = 0;
        s_precision = digits;
    }
    static void useFFT(bool on) { BigInt::useFFT(on); }

    Decimal(): sign(0), scale(0) {}
    Decimal(long long v) {
        if (v == 0) { sign = 0; mant = BigInt(0); scale = 0; }
        else { sign = v < 0 ? -1 : 1; mant = BigInt(v < 0 ? -v : v); scale = 0; }
    }
    explicit Decimal(const string &s) { readFromString(s); }

    friend std::ostream& operator<<(std::ostream &os, const Decimal &x) {
        if (x.sign == 0) { os << '0'; return os; }
        string m = x.mant.toString();
        if (x.scale == 0) {
            if (x.sign < 0) os << '-';
            os << m;
            return os;
        }
        if ((int)m.size() <= x.scale) {
            string pad(x.scale + 1 - (int)m.size(), '0');
            m = pad + m;
        }
        string intpart = m.substr(0, m.size() - x.scale);
        string fracpart = m.substr(m.size() - x.scale);
        int trim = (int)fracpart.size();
        while (trim > 0 && fracpart[trim - 1] == '0') --trim;
        fracpart.resize(trim);
        if (x.sign < 0) os << '-';
        if (intpart.empty()) os << '0'; else os << intpart;
        if (!fracpart.empty()) {
            os << '.';
            os << fracpart;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream &is, Decimal &x) {
        string s; is >> s; x.readFromString(s); return is;
    }

    friend int cmp(const Decimal &a, const Decimal &b) {
        if (a.sign != b.sign) return a.sign < b.sign ? -1 : 1;
        if (a.sign == 0) return 0;
        if (a.scale == b.scale) {
            int c = BigInt::cmp(a.mant, b.mant);
            return a.sign > 0 ? c : -c;
        }
        const Decimal *pa = &a, *pb = &b;
        Decimal ta = a, tb = b;
        if (a.scale < b.scale) {
            int diff = (int)(b.scale - a.scale);
            ta.mant = BigInt::mulInt(ta.mant, pow10Int(diff)); ta.scale = b.scale;
        } else {
            int diff = (int)(a.scale - b.scale);
            tb.mant = BigInt::mulInt(tb.mant, pow10Int(diff)); tb.scale = a.scale;
        }
        int c = BigInt::cmp(ta.mant, tb.mant);
        return a.sign > 0 ? c : -c;
    }
    friend bool operator==(const Decimal &a, const Decimal &b) { return cmp(a, b) == 0; }
    friend bool operator!=(const Decimal &a, const Decimal &b) { return cmp(a, b) != 0; }
    friend bool operator<(const Decimal &a, const Decimal &b) { return cmp(a, b) < 0; }
    friend bool operator<=(const Decimal &a, const Decimal &b) { return cmp(a, b) <= 0; }
    friend bool operator>(const Decimal &a, const Decimal &b) { return cmp(a, b) > 0; }
    friend bool operator>=(const Decimal &a, const Decimal &b) { return cmp(a, b) >= 0; }

    friend Decimal operator+(const Decimal &A, const Decimal &B) {
        if (A.sign == 0) return B;
        if (B.sign == 0) return A;
        Decimal a = A, b = B;
        if (a.scale < b.scale) {
            int diff = (int)(b.scale - a.scale);
            a.mant = BigInt::mulInt(a.mant, pow10Int(diff));
            a.scale = b.scale;
        } else if (b.scale < a.scale) {
            int diff = (int)(a.scale - b.scale);
            b.mant = BigInt::mulInt(b.mant, pow10Int(diff));
            b.scale = a.scale;
        }
        Decimal res;
        if (a.sign == b.sign) {
            res.mant = a.mant + b.mant;
            res.sign = res.mant.isZero() ? 0 : a.sign;
            res.scale = a.scale;
        } else {
            int c = BigInt::cmp(a.mant, b.mant);
            if (c == 0) {
                res = Decimal(); // zero
            } else if (c > 0) {
                res.mant = a.mant - b.mant;
                res.sign = a.sign;
                res.scale = a.scale;
            } else {
                res.mant = b.mant - a.mant;
                res.sign = b.sign;
                res.scale = a.scale;
            }
        }
        res.normalize();
        return res;
    }

    friend Decimal operator-(const Decimal &A, const Decimal &B) {
        Decimal nb = B;
        nb.sign = -nb.sign;
        return A + nb;
    }

    friend Decimal operator*(const Decimal &A, const Decimal &B) {
        if (A.sign == 0 || B.sign == 0) return Decimal();
        Decimal res;
        res.mant = A.mant * B.mant;
        res.sign = A.sign * B.sign;
        res.scale = A.scale + B.scale;
        res.roundAndNormalize(s_precision);
        return res;
    }

    friend Decimal operator/(const Decimal &A, const Decimal &B) {
        if (B.sign == 0) throw std::runtime_error("Decimal division by zero");
        if (A.sign == 0) return Decimal();
        int prec = s_precision;
        int guard = 4;
        int totalScaleUp = prec + guard;
        BigInt num = A.mant;
        num = BigInt::mulInt(num, pow10Int(totalScaleUp));
        auto qr = BigInt::divmod(num, B.mant);
        Decimal res;
        res.mant = qr.first;
        res.sign = A.sign * B.sign;
        res.scale = A.scale - B.scale + totalScaleUp;
        res.roundAndNormalize(prec);
        return res;
    }

    friend Decimal operator%(const Decimal &A, const Decimal &B) {
        if (B.sign == 0) throw std::runtime_error("Decimal modulo by zero");
        if (A.sign == 0) return Decimal();
        Decimal div = A / B;
        Decimal q = div.trunc();
        Decimal prod = q * B;
        Decimal rem = A - prod;
        return rem;
    }

    static Decimal pow(Decimal base, unsigned long long exp) {
        if (exp == 0) return Decimal(1);
        Decimal res(1);
        while (exp) {
            if (exp & 1ULL) res = res * base;
            base = base * base;
            exp >>= 1ULL;
        }
        return res;
    }

    Decimal sqrt() const {
        if (sign < 0) throw std::runtime_error("sqrt of negative");
        if (sign == 0) return Decimal(0);
        int prec = s_precision;
        BigInt target = mant;
        target = BigInt::mulInt(target, pow10Int(2 * prec));
        double approx = std::stod(this->toStringForDouble());
        double x0 = std::sqrt(approx);
        for (int iter = 0; iter < 60; ++iter) {
            Decimal nx = (x + (*this) / x) * Decimal(0.5);
            Decimal diff = nx - x;
            diff.normalize();
            if (diff.isZero() || diff.absLessThanPow10(-prec - 2)) { x = nx; break; }
            x = nx;
        }
        x.roundAndNormalize(prec);
        return x;
    }

    BigInt factorial() const {
        if (scale != 0) throw std::runtime_error("factorial only for integers");
        if (sign < 0) throw std::runtime_error("factorial only for non-negative ints");
        unsigned long long n = 0;
        string s = mant.toString();
        if (s.size() > 20) throw std::runtime_error("factorial too large to compute by this helper");
        n = std::stoull(s);
        return BigInt::factorial((unsigned int)n);
    }

    BigInt gcd(const Decimal &other) const {
        if (scale != 0 || other.scale != 0) throw std::runtime_error("gcd defined for integers only in this template");
        BigInt a = mant, b = other.mant;
        return BigInt::gcd(a, b);
    }

    Decimal trunc() const {
        if (sign == 0) return Decimal();
        if (scale == 0) return *this;
        auto qr = BigInt::divmodInt(mant, pow10Int(scale));
        Decimal r;
        r.mant = qr.first;
        r.sign = sign;
        r.scale = 0;
        return r;
    }

    bool absLessThanPow10(int negExp) const {
        if (negExp >= 0) return false;
        int k = -negExp;
        if (scale < k) return false;
        int left = scale - k;
        if (left == 0) return mant.isZero();
        BigInt pow10 = pow10BigInt(left);
        int c = BigInt::cmp(mant, pow10);
        return c < 0;
    }

    string toStringForDouble() const {
        std::ostringstream oss;
        oss << *this;
        string s = oss.str();
        return s;
    }

private:
    int sign;
    BigInt mant;
    int64_t scale;

    static int s_precision;

    void normalize() {
        if (mant.isZero()) {
            sign = 0; scale = 0; return;
        }
        while (scale > 0) {
            auto qr = BigInt::divmodInt(mant, 10);
            if (qr.second != 0) break;
            mant = qr.first;
            --scale;
        }
    }

    void roundAndNormalize(int keep) {
        if (mant.isZero()) { sign = 0; scale = 0; return; }
        if (scale <= keep) { normalize(); return; }
        int drop = (int)(scale - keep);
        BigInt p10 = pow10BigInt(drop);
        auto qr = BigInt::divmod(mant, p10);
        BigInt q = qr.first;
        BigInt r = qr.second;
        BigInt twiceR = BigInt::mulInt(r, 2);
        if (BigInt::cmp(twiceR, p10) >= 0) q = q + BigInt(1);
        mant = q;
        scale = keep;
        normalize();
    }

    void readFromString(const string &s) {
        string t;
        int n = (int)s.size(), pos = 0;
        while (pos < n && isspace(s[pos])) ++pos;
        int sg = 1;
        if (pos < n && (s[pos] == '+' || s[pos] == '-')) { if (s[pos] == '-') sg = -1; ++pos; }
        string digits;
        int fracDigits = 0;
        bool seenDot = false;
        while (pos < n) {
            char c = s[pos++];
            if (c == '.') { if (seenDot) break; seenDot = true; continue; }
            if (c >= '0' && c <= '9') {
                digits.push_back(c);
                if (seenDot) ++fracDigits;
            } else break;
        }
        int p = 0;
        while (p < (int)digits.size() && digits[p] == '0') ++p;
        if (p == (int)digits.size()) {
            sign = 0; scale = 0; mant = BigInt(0); return;
        }
        string core = digits.substr(p);
        mant = BigInt(core);
        sign = sg;
        scale = fracDigits;
        normalize();
    }

    static BigInt pow10BigInt(int k) {
        if (k < 0) throw std::runtime_error("negative pow10");
        static std::vector<BigInt> cache{ BigInt(1) };
        if ((int)cache.size() > k) return cache[k];
        for (int i = (int)cache.size(); i <= k; ++i) cache.push_back(BigInt::mulInt(cache.back(), 10));
        return cache[k];
    }
    static long long pow10Int(int k) {
        if (k < 0) throw std::runtime_error("negative pow10");
        static const long long LIM = (long long)1e18;
        long long r = 1;
        for (int i = 0; i < k; ++i) {
            if (r > LIM / 10) throw std::overflow_error("pow10Int overflow");
            r *= 10;
        }
        return r;
    }
};

int Decimal::s_precision = 50;