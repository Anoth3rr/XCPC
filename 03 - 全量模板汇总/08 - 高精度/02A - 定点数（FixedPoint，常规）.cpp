/*
用途：动态小数位数的高精度定点数；适合精度会变化、需要 round 的常规场景。
依赖：先粘贴 01A 或 01B 中恰好一个 BigInt。01A 足够常用，01B 适合超大乘法。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

struct FixedPoint {
    BigInt man;
    int sgn;
    int scl;
    static int pre;
    static constexpr int LIM = (1LL << 31) - 1;

  public:
    FixedPoint() : man(0), sgn(0), scl(0) {}
    FixedPoint(int v) { read(std::to_string(v)); }
    FixedPoint(const string &s) { read(s); }
    FixedPoint(const BigInt &m, int sc, int sgn = 1) : man(m), sgn(m.ask0() ? 0 : sgn), scl(sc) {}

    static void setPre(int d) { pre = d < 0 ? 0 : d; }

    static BigInt pow10(int k) {
        if (k < 0)
            throw runtime_error("negative pow10");
        if (k > LIM)
            throw overflow_error("decimal scale too large");
        return BigInt::mul10(BigInt(1), k);
    }

    static BigInt shift(BigInt a, int k) {
        if (k < 0 || k > LIM)
            throw overflow_error("decimal scale out of range");
        return BigInt::mul10(std::move(a), k);
    }

    void norm() {
        if (man.ask0()) {
            sgn = 0;
            scl = 0;
            return;
        }
        while (scl > 0) {
            auto qr = BigInt::divmod(man, 10);
            if (qr.second != 0)
                break;
            man = qr.first;
            --scl;
        }
        if (man.ask0())
            sgn = 0;
    }

    void round(int kp) {
        if (kp < 0)
            throw runtime_error("negative round");
        if (man.ask0()) {
            sgn = 0;
            scl = 0;
            return;
        }
        if (scl <= kp) {
            norm();
            return;
        }

        int drp = scl - kp;
        BigInt p10 = pow10(drp);
        auto qr = BigInt::divmod(man, p10);
        BigInt q = qr.first;
        BigInt r = qr.second;
        BigInt r2 = BigInt::mulInt(r, 2);
        if (BigInt::cmp(r2, p10) >= 0)
            q = q + BigInt(1);
        man = q;
        scl = kp;
        norm();
    }

    FixedPoint abs() const {
        if (sgn == 0)
            return FixedPoint();
        FixedPoint r = *this;
        r.sgn = 1;
        return r;
    }

    void read(const string &s) {
        int n = s.size();
        int i = 0;
        while (i < n && isspace((unsigned char)s[i]))
            ++i;
        int sg = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            if (s[i] == '-')
                sg = -1;
            ++i;
        }
        string d;
        int fd = 0;
        bool dot = false;
        for (; i < n; ++i) {
            char c = s[i];
            if (c == '.') {
                if (dot)
                    break;
                dot = true;
                continue;
            }
            if (c >= '0' && c <= '9') {
                d.push_back(c);
                if (dot)
                    ++fd;
            } else
                break;
        }
        int p = 0;
        while (p < d.size() && d[p] == '0')
            ++p;
        if (p == d.size()) {
            sgn = 0;
            scl = 0;
            man = BigInt(0);
            return;
        }
        string cor = d.substr(p);
        man = BigInt(cor);
        sgn = sg;
        scl = fd;
        norm();
    }

    string str(int p = -1) const {
        if (sgn == 0) {
            if (p <= 0)
                return "0";
            string s = "0.";
            s += string(p, '0');
            return s;
        }
        string ms = man.str();
        if (scl == 0) {
            string out = (sgn < 0 ? "-" : "") + ms;
            if (p > 0)
                out += "." + string(p, '0');
            return out;
        }
        if (ms.size() <= scl)
            ms = string(scl + 1 - ms.size(), '0') + ms;
        string ip = ms.substr(0, ms.size() - scl);
        string fra = ms.substr(ms.size() - scl);
        if (p >= 0) {
            if (fra.size() < p)
                fra += string(p - fra.size(), '0');
            else if (fra.size() > p)
                fra = fra.substr(0, p);
        } else {
            int t = fra.size();
            while (t > 0 && fra[t - 1] == '0')
                --t;
            fra.resize(t);
        }
        string out = (sgn < 0 ? "-" : "") + (ip.empty() ? string("0") : ip);
        if (!fra.empty())
            out += "." + fra;
        else if (p > 0)
            out += "." + string(p, '0');
        return out;
    }

    friend FixedPoint operator+(const FixedPoint &a, const FixedPoint &b) {
        if (a.sgn == 0)
            return b;
        if (b.sgn == 0)
            return a;
        FixedPoint _a = a, _b = b;
        if (_a.scl < _b.scl) {
            int dif = _b.scl - _a.scl;
            _a.man = shift(std::move(_a.man), dif);
            _a.scl = _b.scl;
        } else if (_b.scl < _a.scl) {
            int dif = _a.scl - _b.scl;
            _b.man = shift(std::move(_b.man), dif);
            _b.scl = _a.scl;
        }
        FixedPoint res;
        if (_a.sgn == _b.sgn) {
            res.man = _a.man + _b.man;
            res.sgn = _a.sgn;
            res.scl = _a.scl;
        } else {
            int c = BigInt::cmp(_a.man, _b.man);
            if (c == 0)
                return FixedPoint();
            else if (c > 0) {
                res.man = _a.man - _b.man;
                res.sgn = _a.sgn;
                res.scl = _a.scl;
            } else {
                res.man = _b.man - _a.man;
                res.sgn = _b.sgn;
                res.scl = _a.scl;
            }
        }
        res.round(max<int>(0, pre + 5));
        res.norm();
        return res;
    }

    friend FixedPoint operator-(const FixedPoint &a, const FixedPoint &b) {
        FixedPoint _b = b;
        _b.sgn = -_b.sgn;
        return a + _b;
    }

    friend FixedPoint operator*(const FixedPoint &a, const FixedPoint &b) {
        if (a.sgn == 0 || b.sgn == 0)
            return FixedPoint();
        FixedPoint res;
        res.man = a.man * b.man;
        res.sgn = a.sgn * b.sgn;
        if (a.scl > numeric_limits<int>::max() - b.scl)
            throw overflow_error("scale overflow");
        res.scl = a.scl + b.scl;
        res.round(max<int>(0, pre + 5));
        res.norm();
        return res;
    }

    friend FixedPoint operator/(const FixedPoint &a, const FixedPoint &b) {
        if (b.sgn == 0)
            throw runtime_error("division by zero");
        if (a.sgn == 0)
            return FixedPoint();
        int p = pre, wk = p + 15;
        int e = wk + b.scl - a.scl;
        BigInt num = a.man, den = b.man;
        if (e >= 0) num = shift(std::move(num), e);
        else den = shift(std::move(den), -e);
        auto qr = BigInt::divmod(num, den);
        BigInt q = qr.first;
        FixedPoint res(q, wk, a.sgn * b.sgn);
        res.round(p);
        res.norm();
        return res;
    }

    FixedPoint &operator+=(const FixedPoint &o) {
        *this = *this + o;
        return *this;
    }
    FixedPoint &operator-=(const FixedPoint &o) {
        *this = *this - o;
        return *this;
    }
    FixedPoint &operator*=(const FixedPoint &o) {
        *this = *this * o;
        return *this;
    }
    FixedPoint &operator/=(const FixedPoint &o) {
        *this = *this / o;
        return *this;
    }

    friend bool operator==(const FixedPoint &a, const FixedPoint &b) {
        if (a.sgn != b.sgn)
            return false;
        if (a.sgn == 0)
            return true;
        FixedPoint _a = a, _b = b;
        if (_a.scl < _b.scl) {
            _a.man = shift(std::move(_a.man), _b.scl - _a.scl);
            _a.scl = _b.scl;
        } else if (_b.scl < _a.scl) {
            _b.man = shift(std::move(_b.man), _a.scl - _b.scl);
            _b.scl = _a.scl;
        }
        return _a.man == _b.man;
    }

    friend bool operator!=(const FixedPoint &a, const FixedPoint &b) { return !(a == b); }

    friend bool operator<(const FixedPoint &a, const FixedPoint &b) {
        if (a.sgn != b.sgn)
            return a.sgn < b.sgn;
        if (a.sgn == 0)
            return b.sgn > 0;
        FixedPoint _a = a, _b = b;
        if (_a.scl < _b.scl) {
            _a.man = shift(std::move(_a.man), _b.scl - _a.scl);
            _a.scl = _b.scl;
        } else if (_b.scl < _a.scl) {
            _b.man = shift(std::move(_b.man), _a.scl - _b.scl);
            _b.scl = _a.scl;
        }
        if (_a.sgn > 0)
            return _a.man < _b.man;
        else
            return _a.man > _b.man;
    }

    friend bool operator<=(const FixedPoint &a, const FixedPoint &b) { return !(b < a); }
    friend bool operator>(const FixedPoint &a, const FixedPoint &b) { return b < a; }
    friend bool operator>=(const FixedPoint &a, const FixedPoint &b) { return !(a < b); }

    friend ostream &operator<<(ostream &os, const FixedPoint &x) {
        os << x.str();
        return os;
    }
};

int FixedPoint::pre = 50;
