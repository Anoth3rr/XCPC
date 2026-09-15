/*
用途：有符号大整数，常规赛时版。
特点：10^9 进制、O(n^2) 乘法、规范化长除；适合千至数千位且代码体积可控的场景。
替代：万位级乘法或更大输入使用 01B 的 FFT 版，二者均定义 BigInt，不能同时粘贴。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;
using i128 = __int128_t;

struct BigInt {

    static constexpr int B = 1000000000u;
    static constexpr int W = 9;

    vector<int> d;
    int s = 0;

    BigInt() = default;
    BigInt(int x) {
        *this = x;
    }
    BigInt(const string& a) {
        read(a);
    }

    static u64 mag(int x) {
        return x < 0 ? u64(-(x + 1)) + 1 : u64(x);
    }

    BigInt& operator=(int x) {
        d.clear();
        if (!x) {
            s = 0;
            return *this;
        }
        s = x < 0 ? -1 : 1;
        for (u64 y = mag(x); y; y /= B) d.push_back(y % B);
        return *this;
    }

    BigInt& operator=(const string& a) {
        read(a);
        return *this;
    }

    bool ask0() const {
        return !s;
    }

    void norm() {
        while (!d.empty() && !d.back()) d.pop_back();
        if (d.empty()) s = 0;
    }

    void read(const string& a) {
        d.clear();
        int n = a.size(), p = 0;
        while (p < n && isspace(static_cast<unsigned char>(a[p]))) ++p;
        s = 1;
        if (p < n && (a[p] == '+' || a[p] == '-')) {
            if (a[p] == '-') s = -1;
            ++p;
        }
        while (p < n && a[p] == '0') ++p;
        if (p == n) {
            s = 0;
            return;
        }
        for (int r = n; r > p; r -= W) {
            int l = max<int>(p, r - W);
            int x = 0;
            for (int i = l; i < r; ++i) {
                assert('0' <= a[i] && a[i] <= '9');
                x = x * 10 + a[i] - '0';
            }
            d.push_back(x);
        }
        norm();
    }

    string str() const {
        if (ask0()) return "0";
        string a = s < 0 ? "-" : "";
        a += std::to_string(d.back());
        string b;
        for (int i = d.size() - 1; i--;) {
            b = to_string(d[i]);
            a += string(W - b.size(), '0') + b;
        }
        return a;
    }

    static int cmpAbs(const BigInt& a, const BigInt& b) {
        if (a.d.size() != b.d.size()) return a.d.size() < b.d.size() ? -1 : 1;
        for (int i = a.d.size(); i--;)
            if (a.d[i] != b.d[i]) return a.d[i] < b.d[i] ? -1 : 1;
        return 0;
    }

    static int cmp(const BigInt& a, const BigInt& b) {
        if (a.s != b.s) return a.s < b.s ? -1 : 1;
        if (!a.s) return 0;
        int x = cmpAbs(a, b);
        return a.s > 0 ? x : -x;
    }

    static BigInt addAbs(const BigInt& a, const BigInt& b) {
        BigInt c;
        int n = max<int>(a.d.size(), b.d.size());
        c.d.assign(n, 0);
        u64 w = 0;
        for (int i = 0; i < n; ++i) {
            w += i < a.d.size() ? a.d[i] : 0;
            w += i < b.d.size() ? b.d[i] : 0;
            c.d[i] = w % B;
            w /= B;
        }
        if (w) c.d.push_back(w);
        c.s = 1;
        return c;
    }

    static BigInt subAbs(const BigInt& a, const BigInt& b) {
        BigInt c;
        c.d.assign(a.d.size(), 0);
        int w = 0;
        for (int i = 0; i < a.d.size(); ++i) {
            int x = a.d[i] - (i < b.d.size() ? b.d[i] : 0) - w;
            if (x < 0) x += B, w = 1;
            else w = 0;
            c.d[i] = x;
        }
        c.s = 1;
        c.norm();
        return c;
    }

    friend BigInt operator+(const BigInt& a, const BigInt& b) {
        if (!a.s) return b;
        if (!b.s) return a;
        if (a.s == b.s) {
            BigInt c = addAbs(a, b);
            c.s = a.s;
            return c;
        }
        int x = cmpAbs(a, b);
        if (!x) return BigInt(0);
        BigInt c = x > 0 ? subAbs(a, b) : subAbs(b, a);
        c.s = x > 0 ? a.s : b.s;
        return c;
    }

    friend BigInt operator-(const BigInt& a, const BigInt& b) {
        BigInt c = b;
        c.s = -c.s;
        return a + c;
    }

    static BigInt mul0(const BigInt& a, const BigInt& b) {
        if (!a.s || !b.s) return BigInt(0);
        BigInt c;
        int n = a.d.size(), m = b.d.size();
        c.d.assign(n + m, 0);
        for (int i = 0; i < n; ++i) {
            u64 w = 0;
            for (int j = 0; j < m || w; ++j) {
                u64 x = c.d[i + j] + w;
                if (j < m) x += u64(a.d[i]) * b.d[j];
                c.d[i + j] = x % B;
                w = x / B;
            }
        }
        c.s = a.s * b.s;
        c.norm();
        return c;
    }

    static BigInt mulInt(const BigInt& a, int b) {
        if (!a.s || !b) return BigInt(0);
        BigInt c;
        u64 y = mag(b);
        c.d.assign(a.d.size(), 0);
        i128 w = 0;
        for (int i = 0; i < a.d.size() || w; ++i) {
            if (i == c.d.size()) c.d.push_back(0);
            w += i < a.d.size() ? static_cast<i128>(a.d[i]) * y : 0;
            c.d[i] = w % B;
            w /= B;
        }
        c.s = a.s * (b < 0 ? -1 : 1);
        c.norm();
        return c;
    }

    friend BigInt operator*(const BigInt& a, const BigInt& b) {
        return mul0(a, b);
    }

    static pair<BigInt, int> divmod(const BigInt& a, int b) {
        if (!b) throw runtime_error("division by zero");
        BigInt c;
        c.d.assign(a.d.size(), 0);
        u64 y = mag(b), w = 0;
        for (int i = a.d.size(); i--;) {
            i128 x = static_cast<i128>(w) * B + a.d[i];
            c.d[i] = x / y;
            w = static_cast<u64>(x % y);
        }
        c.s = a.s * (b < 0 ? -1 : 1);
        c.norm();
        int r = w;
        if (a.s < 0) r = -r;
        return {c, r};
    }

    static pair<BigInt, BigInt> divmod(const BigInt& a, const BigInt& b) {
        if (!b.s) throw runtime_error("division by zero");
        if (!a.s) return {BigInt(0), BigInt(0)};
        BigInt x = a, y = b;
        x.s = y.s = 1;
        if (cmpAbs(x, y) < 0) return {BigInt(0), a};
        if (y.d.size() == 1) {
            auto [q, r] = divmod(a, b.s * y.d[0]);
            return {q, BigInt(r)};
        }
        u64 f = u64(B) / (u64(y.d.back()) + 1);
        x = mulInt(x, f);
        y = mulInt(y, f);
        BigInt q;
        q.d.assign(x.d.size(), 0);
        BigInt r;
        for (int i = x.d.size(); i--;) {
            if (r.d.empty()) r.d.push_back(x.d[i]);
            else r.d.insert(r.d.begin(), x.d[i]);
            r.s = 1;
            r.norm();
            u64 u = r.d.size() <= y.d.size() ? 0 : r.d[y.d.size()];
            u64 v = r.d.size() < y.d.size() ? 0 : r.d[y.d.size() - 1];
            u64 z = min<u64>(B - 1, (u64(B) * u + v) / y.d.back());
            r -= mulInt(y, z);
            while (r.s < 0) {
                r += y;
                --z;
            }
            q.d[i] = z;
        }
        q.s = a.s * b.s;
        q.norm();
        r = divmod(r, f).first;
        r.s = a.s;
        r.norm();
        return {q, r};
    }

    friend BigInt operator/(const BigInt& a, const BigInt& b) {
        return divmod(a, b).first;
    }

    friend BigInt operator%(const BigInt& a, const BigInt& b) {
        return divmod(a, b).second;
    }

    static BigInt divBig(const BigInt& a, const BigInt& b) {
        return divmod(a, b).first;
    }

    static BigInt mod(const BigInt& a, const BigInt& b) {
        return divmod(a, b).second;
    }

    BigInt operator/(int x) const {
        return divmod(*this, x).first;
    }

    BigInt operator%(int x) const {
        return BigInt(divmod(*this, x).second);
    }

    static BigInt pow(BigInt a, u64 n) {
        BigInt r(1);
        while (n) {
            if (n & 1) r *= a;
            a *= a;
            n >>= 1;
        }
        return r;
    }

    static BigInt gcd(BigInt a, BigInt b) {
        if (a.s < 0) a.s = 1;
        if (b.s < 0) b.s = 1;
        while (b.s) {
            BigInt r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    static BigInt fac(int n) {
        BigInt r(1);
        for (int i = 2; i <= n; ++i) r = mulInt(r, i);
        return r;
    }

    static BigInt mul1(const BigInt& a, int x) {
        return mulInt(a, x);
    }

    static BigInt mul10(BigInt a, int k) {
        if (k < 0) throw runtime_error("negative decimal shift");
        if (!a.s || !k) return a;
        int q = k / W, r = k % W;
        int p = 1;
        for (int i = 0; i < r; ++i) p *= 10;
        if (r) a = mulInt(a, p);
        if (q) a.d.insert(a.d.begin(), q, 0);
        return a;
    }

    static BigInt div10(BigInt a, int k) {
        if (k < 0) throw runtime_error("negative decimal shift");
        if (!a.s || !k) return a;
        int q = k / W, r = k % W;
        int p = 1;
        for (int i = 0; i < r; ++i) p *= 10;
        if (r) a = divmod(a, p).first;
        if (q >= a.d.size()) return BigInt(0);
        if (q) a.d.erase(a.d.begin(), a.d.begin() + q);
        a.norm();
        return a;
    }

    static void useFFT(bool) {}

    BigInt& operator+=(const BigInt& a) {
        return *this = *this + a;
    }

    BigInt& operator-=(const BigInt& a) {
        return *this = *this - a;
    }

    BigInt& operator*=(const BigInt& a) {
        return *this = *this * a;
    }

    BigInt& operator/=(const BigInt& a) {
        return *this = *this / a;
    }

    friend bool operator==(const BigInt& a, const BigInt& b) {
        return cmp(a, b) == 0;
    }

    friend bool operator!=(const BigInt& a, const BigInt& b) {
        return cmp(a, b) != 0;
    }

    friend bool operator<(const BigInt& a, const BigInt& b) {
        return cmp(a, b) < 0;
    }

    friend bool operator<=(const BigInt& a, const BigInt& b) {
        return cmp(a, b) <= 0;
    }

    friend bool operator>(const BigInt& a, const BigInt& b) {
        return cmp(a, b) > 0;
    }

    friend bool operator>=(const BigInt& a, const BigInt& b) {
        return cmp(a, b) >= 0;
    }

    friend ostream& operator<<(ostream& os, const BigInt& a) {
        return os << a.str();
    }

    friend istream& operator>>(istream& is, BigInt& a) {
        string s;
        is >> s;
        a.read(s);
        return is;
    }
};
