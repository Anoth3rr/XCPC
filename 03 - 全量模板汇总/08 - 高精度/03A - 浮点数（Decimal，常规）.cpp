/*
用途：自动按 setPre 截断的高精度十进制数，支持 sqrt、pow、trunc 等常规接口。
依赖：先粘贴 01A 或 01B 中恰好一个 BigInt；大精度乘法建议选择 01B。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

class Decimal {
  public:
    static void setPre(int d) {
        if (d < 0)
            d = 0;
        pre = d;
    }
    static void useFFT(bool on) { BigInt::useFFT(on); }

    Decimal() : sgn(0), scl(0) {}
    Decimal(int v) { read(std::to_string(v)); }
    explicit Decimal(const string &s) { read(s); }

    friend std::ostream &operator<<(std::ostream &os, const Decimal &x) {
        if (x.sgn == 0) {
            os << '0';
            return os;
        }
        string m = x.man.str();
        if (x.scl == 0) {
            if (x.sgn < 0)
                os << '-';
            os << m;
            return os;
        }
        if (m.size() <= x.scl) {
            string pad(x.scl + 1 - m.size(), '0');
            m = pad + m;
        }
        string ip = m.substr(0, m.size() - x.scl);
        string fp = m.substr(m.size() - x.scl);
        int trm = fp.size();
        while (trm > 0 && fp[trm - 1] == '0')
            --trm;
        fp.resize(trm);
        if (x.sgn < 0)
            os << '-';
        if (ip.empty())
            os << '0';
        else
            os << ip;
        if (!fp.empty()) {
            os << '.';
            os << fp;
        }
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Decimal &x) {
        string s;
        is >> s;
        x.read(s);
        return is;
    }

    string str() const { return strD(); }

    friend int cmp(const Decimal &a, const Decimal &b) {
        if (a.sgn != b.sgn)
            return a.sgn < b.sgn ? -1 : 1;
        if (a.sgn == 0)
            return 0;
        if (a.scl == b.scl) {
            int c = BigInt::cmp(a.man, b.man);
            return a.sgn > 0 ? c : -c;
        }
        Decimal _a = a, _b = b;
        if (a.scl < b.scl) {
            int dif = b.scl - a.scl;
            _a.man = shift(std::move(_a.man), dif);
            _a.scl = b.scl;
        } else {
            int dif = a.scl - b.scl;
            _b.man = shift(std::move(_b.man), dif);
            _b.scl = a.scl;
        }
        int c = BigInt::cmp(_a.man, _b.man);
        return a.sgn > 0 ? c : -c;
    }
    friend bool operator==(const Decimal &a, const Decimal &b) { return cmp(a, b) == 0; }
    friend bool operator!=(const Decimal &a, const Decimal &b) { return cmp(a, b) != 0; }
    friend bool operator<(const Decimal &a, const Decimal &b) { return cmp(a, b) < 0; }
    friend bool operator<=(const Decimal &a, const Decimal &b) { return cmp(a, b) <= 0; }
    friend bool operator>(const Decimal &a, const Decimal &b) { return cmp(a, b) > 0; }
    friend bool operator>=(const Decimal &a, const Decimal &b) { return cmp(a, b) >= 0; }

    friend Decimal operator+(const Decimal &a, const Decimal &b) {
        if (a.sgn == 0)
            return b;
        if (b.sgn == 0)
            return a;
        Decimal _a = a, _b = b;
        if (_a.scl < _b.scl) {
            int dif = _b.scl - _a.scl;
            _a.man = shift(std::move(_a.man), dif);
            _a.scl = _b.scl;
        } else if (_b.scl < _a.scl) {
            int dif = _a.scl - _b.scl;
            _b.man = shift(std::move(_b.man), dif);
            _b.scl = _a.scl;
        }
        Decimal res;
        if (_a.sgn == _b.sgn) {
            res.man = _a.man + _b.man;
            res.sgn = res.man.ask0() ? 0 : _a.sgn;
            res.scl = _a.scl;
        } else {
            int c = BigInt::cmp(_a.man, _b.man);
            if (c == 0) {
                res = Decimal(); // zero
            } else if (c > 0) {
                res.man = _a.man - _b.man;
                res.sgn = _a.sgn;
                res.scl = _a.scl;
            } else {
                res.man = _b.man - _a.man;
                res.sgn = _b.sgn;
                res.scl = _a.scl;
            }
        }
        res.norm();
        return res;
    }

    friend Decimal operator-(const Decimal &a, const Decimal &b) {
        Decimal _b = b;
        _b.sgn = -_b.sgn;
        return a + _b;
    }

    friend Decimal operator*(const Decimal &a, const Decimal &b) {
        if (a.sgn == 0 || b.sgn == 0)
            return Decimal();
        Decimal res;
        res.man = a.man * b.man;
        res.sgn = a.sgn * b.sgn;
        res.scl = a.scl + b.scl;
        res.round(pre);
        return res;
    }

    friend Decimal operator/(const Decimal &a, const Decimal &b) {
        if (b.sgn == 0)
            throw std::runtime_error("Decimal division by zero");
        if (a.sgn == 0)
            return Decimal();
        int prc = pre, wk = prc + 10;
        int e = wk + b.scl - a.scl;
        BigInt num = a.man, den = b.man;
        if (e >= 0) num = shift(std::move(num), e);
        else den = shift(std::move(den), -e);
        auto qr = BigInt::divmod(num, den);
        Decimal res;
        res.man = qr.first;
        res.sgn = a.sgn * b.sgn;
        res.scl = wk;
        res.round(prc);
        return res;
    }

    friend Decimal operator%(const Decimal &a, const Decimal &b) {
        if (b.sgn == 0)
            throw std::runtime_error("Decimal modulo by zero");
        if (a.sgn == 0)
            return Decimal();
        Decimal div = a / b;
        Decimal q = div.trunc();
        Decimal prd = q * b;
        Decimal rem = a - prd;
        return rem;
    }

    Decimal& operator+=(const Decimal& a) { return *this = *this + a; }
    Decimal& operator-=(const Decimal& a) { return *this = *this - a; }
    Decimal& operator*=(const Decimal& a) { return *this = *this * a; }
    Decimal& operator/=(const Decimal& a) { return *this = *this / a; }
    Decimal& operator%=(const Decimal& a) { return *this = *this % a; }

    Decimal abs() const {
        Decimal a = *this;
        if (a.sgn < 0) a.sgn = 1;
        return a;
    }

    static Decimal pow(Decimal bas, int exp) {
        if (exp == 0)
            return Decimal(1);
        Decimal res(1);
        while (exp) {
            if (exp & 1)
                res = res * bas;
            bas = bas * bas;
            exp >>= 1;
        }
        return res;
    }

    Decimal sqrt() const {
        if (sgn < 0)
            throw std::runtime_error("sqrt of negative");
        if (sgn == 0)
            return Decimal(0);
        int prc = pre;
        int e = man.str().size() - scl;
        int t = e >= 0 ? e / 2 : -((-e + 1) / 2);
        if (std::abs(t) > LIM)
            throw std::overflow_error("sqrt exponent too large");
        string s;
        if (t >= 0) s = "1" + string(t, '0');
        else s = "0." + string(-t - 1, '0') + "1";
        Decimal x(s);
        for (int it = 0; it < 60; ++it) {
            Decimal nx = (x + (*this) / x) * Decimal("0.5");
            Decimal dif = nx - x;
            dif.norm();
            if (dif.ask0() || dif.small(-prc - 2)) {
                x = nx;
                break;
            }
            x = nx;
        }
        x.round(prc);
        return x;
    }

    BigInt fac() const {
        if (scl != 0)
            throw std::runtime_error("factorial only for integers");
        if (sgn < 0)
            throw std::runtime_error("factorial only for non-negative ints");
        int n = 0;
        string s = man.str();
        if (s.size() > 20)
            throw std::runtime_error("factorial too large to compute by this helper");
        n = std::stoull(s);
        return BigInt::fac(n);
    }

    BigInt gcd(const Decimal &o) const {
        if (scl != 0 || o.scl != 0)
            throw std::runtime_error("gcd defined for integers only in this template");
        BigInt a = man, b = o.man;
        return BigInt::gcd(a, b);
    }

    Decimal trunc() const {
        if (sgn == 0)
            return Decimal();
        if (scl == 0)
            return *this;
        auto qr = BigInt::divmod(man, pow10(scl));
        Decimal r;
        r.man = qr.first;
        r.sgn = sgn;
        r.scl = 0;
        return r;
    }

    bool ask0() const {
        return sgn == 0;
    }

    bool small(int e) const {
        if (e >= 0)
            return false;
        int k = -e;
        if (scl < k)
            return false;
        int l = scl - k;
        if (l == 0)
            return man.ask0();
        BigInt p10 = pow10(l);
        int c = BigInt::cmp(man, p10);
        return c < 0;
    }

    string strD() const {
        std::ostringstream oss;
        oss << *this;
        string s = oss.str();
        return s;
    }

  private:
    int sgn;
    BigInt man;
    int scl;

    static int pre;
    static constexpr int LIM = (1LL << 31) - 1;

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
    }

    void round(int kp) {
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
        BigInt rr = BigInt::mulInt(r, 2);
        if (BigInt::cmp(rr, p10) >= 0)
            q = q + BigInt(1);
        man = q;
        scl = kp;
        norm();
    }

    void read(const string &s) {
        string t;
        int n = s.size(), pos = 0;
        while (pos < n && isspace(s[pos]))
            ++pos;
        int sg = 1;
        if (pos < n && (s[pos] == '+' || s[pos] == '-')) {
            if (s[pos] == '-')
                sg = -1;
            ++pos;
        }
        string d;
        int fd = 0;
        bool dot = false;
        while (pos < n) {
            char c = s[pos++];
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

    static BigInt shift(BigInt a, int k) {
        if (k < 0 || k > LIM)
            throw std::overflow_error("decimal scale out of range");
        return BigInt::mul10(std::move(a), k);
    }

    static BigInt pow10(int k) {
        if (k < 0)
            throw std::runtime_error("negative pow10");
        return shift(BigInt(1), k);
    }
    static int pow10i(int k) {
        if (k < 0)
            throw std::runtime_error("negative pow10");
        static const int mx = 1'000'000'000'000'000'000LL;
        int r = 1;
        for (int i = 0; i < k; ++i) {
            if (r > mx / 10)
                throw std::overflow_error("pow10Int overflow");
            r *= 10;
        }
        return r;
    }
};

int Decimal::pre = 50;
