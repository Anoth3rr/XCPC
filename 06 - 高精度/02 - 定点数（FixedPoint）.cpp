struct FixedPoint {
    BigInt mant;
    int sign;
    int scale;
    static int DEFAULT_PRECISION;

  private:
    static vector<BigInt> pow10_cache;

  public:
    FixedPoint() : mant(0), sign(0), scale(0) {}
    FixedPoint(long long v) {
        if (v == 0) {
            mant = BigInt(0);
            sign = 0;
            scale = 0;
        } else {
            mant = BigInt(v < 0 ? -v : v);
            sign = v < 0 ? -1 : 1;
            scale = 0;
        }
    }
    FixedPoint(const string &s) { fromString(s); }
    FixedPoint(const BigInt &m, int sc, int sgn = 1) : mant(m), scale(sc), sign(m.isZero() ? 0 : sgn) {}

    static void setDefaultPrecision(int p) { DEFAULT_PRECISION = p < 0 ? 0 : p; }

    static BigInt pow10BigInt(int k) {
        if (k < 0)
            throw runtime_error("pow10BigInt negative");
        if ((int)pow10_cache.size() > k)
            return pow10_cache[k];
        if (pow10_cache.empty())
            pow10_cache.push_back(BigInt(1));
        for (int i = (int)pow10_cache.size(); i <= k; ++i) {
            pow10_cache.push_back(BigInt::mulInt(pow10_cache.back(), 10));
        }
        return pow10_cache[k];
    }

    void normalize() {
        if (mant.isZero()) {
            sign = 0;
            scale = 0;
            return;
        }
        while (scale > 0) {
            auto qr = BigInt::divmodInt(mant, 10);
            if (qr.second != 0)
                break;
            mant = qr.first;
            --scale;
        }
        if (mant.isZero())
            sign = 0;
    }

    void roundKeep(int keep) {
        if (keep < 0)
            throw runtime_error("roundKeep negative");
        if (mant.isZero()) {
            sign = 0;
            scale = 0;
            return;
        }
        if (scale <= keep) {
            normalize();
            return;
        }

        int drop = scale - keep;
        BigInt p10 = pow10BigInt(drop);
        auto qr = BigInt::divmod(mant, p10);
        BigInt q = qr.first;
        BigInt r = qr.second;
        BigInt twoR = BigInt::mulInt(r, 2LL);
        if (BigInt::cmp(twoR, p10) >= 0)
            q = q + BigInt(1);
        mant = q;
        scale = keep;
        normalize();
    }

    FixedPoint abs() const {
        if (sign == 0)
            return FixedPoint();
        FixedPoint r = *this;
        r.sign = 1;
        return r;
    }

    void fromString(const string &s) {
        int n = (int)s.size();
        int i = 0;
        while (i < n && isspace((unsigned char)s[i]))
            ++i;
        int sg = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            if (s[i] == '-')
                sg = -1;
            ++i;
        }
        string digits;
        int fracDigits = 0;
        bool seenDot = false;
        for (; i < n; ++i) {
            char c = s[i];
            if (c == '.') {
                if (seenDot)
                    break;
                seenDot = true;
                continue;
            }
            if (c >= '0' && c <= '9') {
                digits.push_back(c);
                if (seenDot)
                    ++fracDigits;
            } else
                break;
        }
        int p = 0;
        while (p < (int)digits.size() && digits[p] == '0')
            ++p;
        if (p == (int)digits.size()) {
            sign = 0;
            scale = 0;
            mant = BigInt(0);
            return;
        }
        string core = digits.substr(p);
        mant = BigInt(core);
        sign = sg;
        scale = fracDigits;
        normalize();
    }

    string toString(int outScale = -1) const {
        if (sign == 0) {
            if (outScale <= 0)
                return "0";
            string s = "0.";
            s += string(outScale, '0');
            return s;
        }
        string ms = mant.toString();
        if (scale == 0) {
            string out = (sign < 0 ? "-" : "") + ms;
            if (outScale > 0)
                out += "." + string(outScale, '0');
            return out;
        }
        if ((int)ms.size() <= scale)
            ms = string(scale + 1 - (int)ms.size(), '0') + ms;
        string intpart = ms.substr(0, ms.size() - scale);
        string frac = ms.substr(ms.size() - scale);
        if (outScale >= 0) {
            if ((int)frac.size() < outScale)
                frac += string(outScale - frac.size(), '0');
            else if ((int)frac.size() > outScale)
                frac = frac.substr(0, outScale);
        } else {
            int t = (int)frac.size();
            while (t > 0 && frac[t - 1] == '0')
                --t;
            frac.resize(t);
        }
        string out = (sign < 0 ? "-" : "") + (intpart.empty() ? string("0") : intpart);
        if (!frac.empty())
            out += "." + frac;
        else if (outScale > 0)
            out += "." + string(outScale, '0');
        return out;
    }

    friend FixedPoint operator+(const FixedPoint &A, const FixedPoint &B) {
        if (A.sign == 0)
            return B;
        if (B.sign == 0)
            return A;
        FixedPoint a = A, b = B;
        if (a.scale < b.scale) {
            int diff = b.scale - a.scale;
            a.mant = a.mant * pow10BigInt(diff);
            a.scale = b.scale;
        } else if (b.scale < a.scale) {
            int diff = a.scale - b.scale;
            b.mant = b.mant * pow10BigInt(diff);
            b.scale = a.scale;
        }
        FixedPoint res;
        if (a.sign == b.sign) {
            res.mant = a.mant + b.mant;
            res.sign = a.sign;
            res.scale = a.scale;
        } else {
            int c = BigInt::cmp(a.mant, b.mant);
            if (c == 0)
                return FixedPoint();
            else if (c > 0) {
                res.mant = a.mant - b.mant;
                res.sign = a.sign;
                res.scale = a.scale;
            } else {
                res.mant = b.mant - a.mant;
                res.sign = b.sign;
                res.scale = a.scale;
            }
        }
        res.roundKeep(max(0, DEFAULT_PRECISION + 5));
        res.normalize();
        return res;
    }

    friend FixedPoint operator-(const FixedPoint &A, const FixedPoint &B) {
        FixedPoint nb = B;
        nb.sign = -nb.sign;
        return A + nb;
    }

    friend FixedPoint operator*(const FixedPoint &A, const FixedPoint &B) {
        if (A.sign == 0 || B.sign == 0)
            return FixedPoint();
        FixedPoint res;
        res.mant = A.mant * B.mant;
        res.sign = A.sign * B.sign;
        long long ssum = (long long)A.scale + (long long)B.scale;
        if (ssum > INT_MAX)
            throw overflow_error("scale overflow");
        res.scale = (int)ssum;
        res.roundKeep(max(0, DEFAULT_PRECISION + 5));
        res.normalize();
        return res;
    }

    friend FixedPoint operator/(const FixedPoint &A, const FixedPoint &B) {
        if (B.sign == 0)
            throw runtime_error("division by zero");
        if (A.sign == 0)
            return FixedPoint();
        int prec = DEFAULT_PRECISION;
        int guard = 15;
        int totalUp = prec + guard;
        BigInt num = A.mant * pow10BigInt(totalUp);
        auto qr = BigInt::divmod(num, B.mant);
        BigInt q = qr.first;
        FixedPoint res(q, A.scale - B.scale + totalUp, A.sign * B.sign);
        res.roundKeep(prec);
        res.normalize();
        return res;
    }

    friend bool operator==(const FixedPoint &A, const FixedPoint &B) {
        if (A.sign != B.sign)
            return false;
        if (A.sign == 0)
            return true;
        FixedPoint a = A, b = B;
        if (a.scale < b.scale) {
            a.mant = a.mant * pow10BigInt(b.scale - a.scale);
            a.scale = b.scale;
        } else if (b.scale < a.scale) {
            b.mant = b.mant * pow10BigInt(a.scale - b.scale);
            b.scale = a.scale;
        }
        return a.mant == b.mant;
    }

    friend bool operator!=(const FixedPoint &A, const FixedPoint &B) { return !(A == B); }

    friend bool operator<(const FixedPoint &A, const FixedPoint &B) {
        if (A.sign != B.sign)
            return A.sign < B.sign;
        if (A.sign == 0)
            return B.sign > 0;
        FixedPoint a = A, b = B;
        if (a.scale < b.scale) {
            a.mant = a.mant * pow10BigInt(b.scale - a.scale);
            a.scale = b.scale;
        } else if (b.scale < a.scale) {
            b.mant = b.mant * pow10BigInt(a.scale - b.scale);
            b.scale = a.scale;
        }
        if (a.sign > 0)
            return a.mant < b.mant;
        else
            return a.mant > b.mant;
    }

    friend bool operator<=(const FixedPoint &A, const FixedPoint &B) { return !(B < A); }
    friend bool operator>(const FixedPoint &A, const FixedPoint &B) { return B < A; }
    friend bool operator>=(const FixedPoint &A, const FixedPoint &B) { return !(A < B); }

    friend ostream &operator<<(ostream &os, const FixedPoint &x) {
        os << x.toString();
        return os;
    }
};

int FixedPoint::DEFAULT_PRECISION = 50;
vector<BigInt> FixedPoint::pow10_cache = {BigInt(1)};