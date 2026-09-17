/*
用途：编译期固定 P 位小数的高精度定点数；频繁四则运算时避免动态对齐、去零和幂缓存。
依赖：先粘贴 01B（推荐）或 01A 中恰好一个 BigInt。P 是小数位数，构造和乘除均向 0 截断。
示例：using F = FastFixedPoint<1000>; F a("1.25"), b("3");
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <int P>
struct FixedPoint {
    static_assert(P >= 0);

    BigInt x;

    FixedPoint() : x(0) {}
    FixedPoint(int a) : x(BigInt::mul10(BigInt(a), P)) {}
    explicit FixedPoint(const string& s) { read(s); }

    static FixedPoint raw(BigInt x) {
        FixedPoint a;
        a.x = std::move(x);
        return a;
    }

    bool ask0() const { return x.ask0(); }

    FixedPoint abs() const {
        return x < BigInt(0) ? raw(BigInt(0) - x) : *this;
    }

    void read(const string& s) {
        int n = s.size(), i = 0, f = 0;
        while (i < n && isspace(static_cast<unsigned char>(s[i]))) ++i;
        int sg = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            if (s[i] == '-') sg = -1;
            ++i;
        }
        string a;
        bool dot = false;
        for (; i < n; ++i) {
            char c = s[i];
            if (c == '.') {
                if (dot) break;
                dot = true;
            } else if ('0' <= c && c <= '9') {
                a += c;
                if (dot) ++f;
            } else {
                break;
            }
        }
        int p = 0;
        while (p < a.size() && a[p] == '0') ++p;
        if (p == a.size()) {
            x = BigInt(0);
            return;
        }
        BigInt z(a.substr(p));
        if (f < P) z = BigInt::mul10(std::move(z), P - f);
        if (f > P) {
            BigInt q = BigInt::mul10(BigInt(1), f - P);
            auto [u, v] = BigInt::divmod(z, q);
            if (BigInt::cmp(BigInt::mulInt(v, 2), q) >= 0) u += BigInt(1);
            z = std::move(u);
        }
        x = sg < 0 ? BigInt(0) - z : z;
    }

    string str(int o = -1) const {
        if (x.ask0()) {
            if (o <= 0) return "0";
            return "0." + string(o, '0');
        }
        string a = x.str();
        bool neg = a[0] == '-';
        if (neg) a.erase(a.begin());
        if constexpr (P == 0) {
            if (o > 0) a += "." + string(o, '0');
            return (neg ? "-" : "") + a;
        }
        if (a.size() <= P)
            a = string(P + 1 - a.size(), '0') + a;
        string ip = a.substr(0, a.size() - P);
        string fp = a.substr(a.size() - P);
        if (o >= 0) {
            if (o < P) fp.resize(o);
            else fp += string(o - P, '0');
        } else {
            while (!fp.empty() && fp.back() == '0') fp.pop_back();
        }
        string r = (neg ? "-" : "") + ip;
        if (!fp.empty()) r += "." + fp;
        else if (o > 0) r += "." + string(o, '0');
        return r;
    }

    friend FixedPoint operator+(const FixedPoint& a, const FixedPoint& b) {
        return raw(a.x + b.x);
    }

    friend FixedPoint operator-(const FixedPoint& a, const FixedPoint& b) {
        return raw(a.x - b.x);
    }

    friend FixedPoint operator*(const FixedPoint& a, const FixedPoint& b) {
        return raw(BigInt::div10(a.x * b.x, P));
    }

    friend FixedPoint operator/(const FixedPoint& a, const FixedPoint& b) {
        if (b.x.ask0()) throw runtime_error("division by zero");
        return raw(BigInt::mul10(a.x, P) / b.x);
    }

    friend FixedPoint operator%(const FixedPoint& a, const FixedPoint& b) {
        if (b.x.ask0()) throw runtime_error("division by zero");
        return raw(a.x % b.x);
    }

    FixedPoint& operator+=(const FixedPoint& a) { return *this = *this + a; }
    FixedPoint& operator-=(const FixedPoint& a) { return *this = *this - a; }
    FixedPoint& operator*=(const FixedPoint& a) { return *this = *this * a; }
    FixedPoint& operator/=(const FixedPoint& a) { return *this = *this / a; }
    FixedPoint& operator%=(const FixedPoint& a) { return *this = *this % a; }

    static FixedPoint pow(FixedPoint a, int n) {
        FixedPoint r(1);
        while (n) {
            if (n & 1) r *= a;
            a *= a;
            n >>= 1;
        }
        return r;
    }

    friend bool operator==(const FixedPoint& a, const FixedPoint& b) { return a.x == b.x; }
    friend bool operator!=(const FixedPoint& a, const FixedPoint& b) { return a.x != b.x; }
    friend bool operator<(const FixedPoint& a, const FixedPoint& b) { return a.x < b.x; }
    friend bool operator<=(const FixedPoint& a, const FixedPoint& b) { return a.x <= b.x; }
    friend bool operator>(const FixedPoint& a, const FixedPoint& b) { return a.x > b.x; }
    friend bool operator>=(const FixedPoint& a, const FixedPoint& b) { return a.x >= b.x; }

    friend ostream& operator<<(ostream& os, const FixedPoint& a) {
        return os << a.str();
    }

    friend istream& operator>>(istream& is, FixedPoint& a) {
        string s;
        is >> s;
        a.read(s);
        return is;
    }
};
