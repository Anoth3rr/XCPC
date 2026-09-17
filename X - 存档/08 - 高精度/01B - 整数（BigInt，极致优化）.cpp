/*
用途：有符号大整数，FFT 自动切换的极致优化版。
特点：10^9 进制、长除、超大乘法自动使用 FFT；与 01A 同名 BigInt，二者不能同时粘贴。
性能：十进制块和 FFT 拆分数组保留 i32/u32，并把两组实系数打包进一次正变换；
      改成宏展开后的 int 或拆成两次正变换都会显著增加内存与常数。
*/

using i32 = int32_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i128 = __int128_t;

struct BigInt {
  public:
    static constexpr u32 B = 1000000000u;
    static constexpr i32 D = 9;

  private:
    static constexpr double PI = 3.141592653589793238462643383279502884;

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

        Complex &operator/=(T v) {
            a /= v;
            b /= v;
            return *this;
        }
    };
    using cd = Complex<double>;

    static void fft(vector<cd> &a, bool inv) {
        i32 n = a.size();
        static vector<i32> rev;
        static vector<cd> rts{{0, 0}, {1, 0}};

        i32 rn = rev.size();
        if (rn != n) {
            i32 k = __builtin_ctz(u32(n));
            rev.assign(n, 0);
            for (i32 i = 0; i < n; i++)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (k - 1));
        }

        i32 sn = rts.size();
        if (sn < n) {
            i32 k = __builtin_ctz(u32(sn));
            rts.resize(n);
            while ((i32(1) << k) < n) {
                double da = 2 * PI / (i32(1) << (k + 1));
                for (i32 i = i32(1) << (k - 1); i < (i32(1) << k); i++) {
                    rts[i << 1] = rts[i];
                    double ag = da * (2 * i + 1 - (i32(1) << k));
                    rts[i << 1 | 1] = cd(cos(ag), sin(ag));
                }
                k++;
            }
        }

        for (i32 i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);

        for (i32 len = 1; len < n; len <<= 1) {
            for (i32 i = 0; i < n; i += len << 1) {
                for (i32 j = 0; j < len; j++) {
                    cd u = a[i + j];
                    cd v = a[i + j + len] * rts[len + j];
                    a[i + j] = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }

        if (inv) {
            reverse(a.begin() + 1, a.end());
            for (cd &x : a)
                x /= n;
        }
    }

    static BigInt mulFFT(const BigInt &a, const BigInt &b) {
        if (a.ask0() || b.ask0()) return BigInt(0);
        constexpr i32 FB = 1000;
        constexpr i32 FD = 3;
        vector<i32> fa, fb;
        i32 na = a.d.size(), nb = b.d.size();
        fa.reserve(na * 3);
        fb.reserve(nb * 3);
        for (i32 i = 0; i < na; ++i) {
            u32 x = a.d[i];
            for (i32 k = 0; k < D / FD; ++k) {
                fa.push_back(x % FB);
                x /= FB;
            }
        }
        for (i32 i = 0; i < nb; ++i) {
            u32 x = b.d[i];
            for (i32 k = 0; k < D / FD; ++k) {
                fb.push_back(x % FB);
                x /= FB;
            }
        }
        while (!fa.empty() && !fa.back()) fa.pop_back();
        while (!fb.empty() && !fb.back()) fb.pop_back();

        i32 n = 1;
        i32 fn = fa.size(), gn = fb.size();
        while (n < fn + gn) n <<= 1;
        vector<cd> x(n), y(n);
        for (i32 i = 0; i < fn; ++i) x[i].a = fa[i];
        for (i32 i = 0; i < gn; ++i) x[i].b = fb[i];
        fft(x, false);
        for (i32 i = 0; i < n; ++i) {
            i32 j = (n - i) & (n - 1);
            cd p{(x[i].a + x[j].a) * 0.5, (x[i].b - x[j].b) * 0.5};
            cd q{(x[i].b + x[j].b) * 0.5, (x[j].a - x[i].a) * 0.5};
            y[i] = p * q;
        }
        fft(y, true);

        vector<i32> c;
        c.reserve(n + 4);
        u64 cy = 0;
        for (i32 i = 0; i < n; ++i) {
            int v = y[i].a + 0.5;
            if (v < 0) v = 0;
            u64 w = cy + static_cast<u64>(v);
            c.push_back(static_cast<i32>(w % FB));
            cy = w / FB;
        }
        while (cy) {
            c.push_back(static_cast<i32>(cy % FB));
            cy /= FB;
        }
        while (!c.empty() && !c.back()) c.pop_back();

        BigInt res;
        i32 cn = c.size();
        res.d.reserve((cn + 2) / 3);
        for (i32 i = 0; i < cn; i += 3) {
            u64 w = 0, p = 1;
            for (i32 j = 0; j < 3 && i + j < cn; ++j) {
                w += static_cast<u64>(c[i + j]) * p;
                p *= FB;
            }
            res.d.push_back(static_cast<u32>(w));
        }
        res.sgn = a.sgn * b.sgn;
        res.norm();
        return res;
    }

    vector<u32> d;
    short sgn;

  public:
    BigInt() : sgn(0) {}
    BigInt(int v) { *this = v; }
    BigInt(const string &s) { read(s); }
    static u64 mag(int v) {
        return v < 0 ? u64(-(v + 1)) + 1 : u64(v);
    }
    BigInt &operator=(int v) {
        d.clear();
        if (v == 0) {
            sgn = 0;
            return *this;
        }
        sgn = v < 0 ? -1 : 1;
        u64 av = mag(v);
        while (av) {
            d.push_back(u32(av % B));
            av /= B;
        }
        return *this;
    }

    BigInt &operator=(const string &s) {
        read(s);
        return *this;
    }

    bool ask0() const { return sgn == 0; }

    void norm() {
        while (!d.empty() && d.back() == 0)
            d.pop_back();
        if (d.empty())
            sgn = 0;
    }

    string str() const {
        if (ask0()) return "0";
        string s = sgn < 0 ? "-" : "";
        s += std::to_string(d.back());
        char buf[32];
        for (i32 i = d.size() - 1; i--;) {
            snprintf(buf, sizeof(buf), "%09u", d[i]);
            s += buf;
        }
        return s;
    }

    void read(const string &s) {
        d.clear();
        i32 n = s.size(), p = 0, e = n;
        while (p < n && isspace(static_cast<unsigned char>(s[p]))) ++p;
        sgn = 1;
        if (p < n && (s[p] == '+' || s[p] == '-')) {
            if (s[p] == '-') sgn = -1;
            ++p;
        }
        while (e > p && isspace(static_cast<unsigned char>(s[e - 1]))) --e;
        while (p < e && s[p] == '0') ++p;
        if (p == e) {
            sgn = 0;
            return;
        }
        for (i32 i = e; i > p; i -= D) {
            u32 x = 0;
            i32 l = max<i32>(p, i - D);
            for (i32 j = l; j < i; ++j) {
                assert('0' <= s[j] && s[j] <= '9');
                x = x * 10 + (s[j] - '0');
            }
            d.push_back(x);
        }

        norm();
    }

  public:
    static i32 cmpAbs(const BigInt &a, const BigInt &b) {
        if (a.d.size() != b.d.size())
            return a.d.size() < b.d.size() ? -1 : 1;
        for (i32 i = a.d.size(); i--;)
            if (a.d[i] != b.d[i])
                return a.d[i] < b.d[i] ? -1 : 1;
        return 0;
    }
    static i32 cmp(const BigInt &a, const BigInt &b) {
        if (a.sgn != b.sgn)
            return a.sgn < b.sgn ? -1 : 1;
        if (a.sgn == 0)
            return 0;
        i32 c = cmpAbs(a, b);
        return a.sgn > 0 ? c : -c;
    }

  private:
    static BigInt addAbs(const BigInt &a, const BigInt &b) {
        BigInt res;
        u64 cy = 0;
        i32 na = a.d.size(), nb = b.d.size();
        i32 n = max(na, nb);
        res.d.assign(n, 0);
        for (i32 i = 0; i < n; ++i) {
            u64 av = i < na ? a.d[i] : 0;
            u64 bv = i < nb ? b.d[i] : 0;
            u64 sum = av + bv + cy;
            res.d[i] = u32(sum % B);
            cy = sum / B;
        }
        if (cy)
            res.d.push_back(u32(cy));
        res.sgn = res.d.empty() ? 0 : 1;
        return res;
    }

    static BigInt subAbs(const BigInt &a, const BigInt &b) {
        BigInt res;
        res.d.assign(a.d.size(), 0);
        u64 cy = 0;
        i32 na = a.d.size(), nb = b.d.size();
        for (i32 i = 0; i < na; ++i) {
            u64 av = a.d[i];
            u64 bv = i < nb ? b.d[i] : 0;
            int cur = static_cast<int>(av) - static_cast<int>(bv) - static_cast<int>(cy);
            if (cur < 0) {
                cur += B;
                cy = 1;
            } else
                cy = 0;
            res.d[i] = u32(cur);
        }
        res.norm();
        res.sgn = res.d.empty() ? 0 : 1;
        return res;
    }

  public:
    BigInt operator+(const BigInt &v) const {
        if (sgn == 0)
            return v;
        if (v.sgn == 0)
            return *this;
        BigInt res;
        if (sgn == v.sgn) {
            res = addAbs(*this, v);
            res.sgn = sgn;
        } else {
            i32 cmp = cmpAbs(*this, v);
            if (cmp >= 0) {
                res = subAbs(*this, v);
                res.sgn = sgn;
            } else {
                res = subAbs(v, *this);
                res.sgn = v.sgn;
            }
        }
        if (res.ask0())
            res.sgn = 0;
        return res;
    }

    BigInt operator-(const BigInt &v) const {
        if (v.ask0())
            return *this;
        BigInt tmp = v;
        tmp.sgn = -tmp.sgn;
        return *this + tmp;
    }

    static BigInt mul0(const BigInt &a, const BigInt &b) {
        if (a.ask0() || b.ask0())
            return BigInt(0);
        BigInt res;
        i32 n = a.d.size(), m = b.d.size();
        res.d.assign(n + m + 1, 0);
        for (i32 i = 0; i < n; ++i) {
            u64 cy = 0;
            for (i32 j = 0; j < m || cy; ++j) {
                u64 cur = res.d[i + j] + cy + u64(a.d[i]) * (j < m ? b.d[j] : 0);
                res.d[i + j] = u32(cur % B);
                cy = cur / B;
            }
        }
        res.sgn = a.sgn * b.sgn;
        res.norm();
        return res;
    }

    static BigInt mulInt(const BigInt &a, int m) {
        if (a.ask0() || m == 0)
            return BigInt(0);
        BigInt res;
        short sg2 = m < 0 ? -1 : 1;
        u64 mm = mag(m);
        res.d.assign(a.d.size(), 0);
        u64 cy = 0;
        i32 n = a.d.size();
        for (i32 i = 0; i < n || cy; ++i) {
            i128 cur = cy + static_cast<i128>(i < n ? a.d[i] : 0) * mm;
            if (i == n)
                res.d.push_back(0);
            res.d[i] = static_cast<u32>(cur % B);
            cy = static_cast<u64>(cur / B);
        }
        res.sgn = a.sgn * sg2;
        res.norm();
        return res;
    }

  public:
    static constexpr i32 lim = 1500;
    BigInt operator*(const BigInt &v) const {
        if (ask0() || v.ask0())
            return BigInt(0);
        i32 n = d.size(), m = v.d.size();
        if (min(n, m) == 0)
            return BigInt(0);
        if (n + m < lim)
            return mul0(*this, v);
        else
            return mulFFT(*this, v);
    }

    static pair<BigInt, int> divmod(const BigInt &a, int b) {
        if (b == 0)
            throw runtime_error("div by zero");
        BigInt q;
        q.d.assign(a.d.size(), 0);
        u64 cy = 0, ub = mag(b);
        for (i32 i = a.d.size(); i--;) {
            i128 cur = static_cast<i128>(cy) * B + a.d[i];
            q.d[i] = static_cast<u32>(cur / ub);
            cy = static_cast<u64>(cur % ub);
        }
        q.sgn = (q.d.empty() ? 0 : (a.sgn * (b < 0 ? -1 : 1)));
        q.norm();
        int rem = cy;
        if (a.sgn < 0)
            rem = -rem;
        return {q, rem};
    }

    static pair<BigInt, BigInt> divmod(const BigInt &a, const BigInt &b) {
        if (b.ask0())
            throw runtime_error("div by zero");
        if (a.ask0())
            return {BigInt(0), BigInt(0)};
        i32 sgn = a.sgn * b.sgn;
        BigInt aa = a;
        aa.sgn = 1;
        BigInt bb = b;
        bb.sgn = 1;
        if (cmpAbs(aa, bb) < 0)
            return {BigInt(0), a};

        if (bb.d.size() == 1) {
            auto [q, r] = divmod(a, static_cast<int>(b.sgn) * bb.d[0]);
            return {q, BigInt(r)};
        }
        u64 f = u64(B) / (u64(bb.d.back()) + 1);
        aa = mulInt(aa, f);
        bb = mulInt(bb, f);
        BigInt q;
        q.d.assign(aa.d.size(), 0);
        BigInt r;
        for (i32 i = aa.d.size(); i--;) {
            if (r.d.empty()) r.d.push_back(aa.d[i]);
            else r.d.insert(r.d.begin(), aa.d[i]);
            r.sgn = 1;
            r.norm();
            u64 x = r.d.size() <= bb.d.size() ? 0 : r.d[bb.d.size()];
            u64 y = r.d.size() < bb.d.size() ? 0 : r.d[bb.d.size() - 1];
            u64 z = min<u64>(B - 1, (B * x + y) / bb.d.back());
            r -= mulInt(bb, z);
            while (r.sgn < 0) {
                r += bb;
                --z;
            }
            q.d[i] = static_cast<u32>(z);
        }
        q.sgn = sgn;
        q.norm();
        r = divmod(r, f).first;
        r.sgn = a.sgn;
        r.norm();
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
    BigInt operator/(int v) const { return divmod(*this, v).first; }
    BigInt operator%(int v) const { return BigInt(divmod(*this, v).second); }

    static BigInt pow(BigInt a, u64 e) {
        BigInt res(1);
        while (e) {
            if (e & 1)
                res = res * a;
            a = a * a;
            e >>= 1;
        }
        return res;
    }

    static void useFFT(bool) {}

    static BigInt gcd(BigInt a, BigInt b) {
        if (a.sgn < 0) a.sgn = 1;
        if (b.sgn < 0) b.sgn = 1;
        while (!b.ask0()) {
            BigInt r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    static BigInt fac(int n) {
        BigInt res(1);
        for (int i = 2; i <= n; i++) {
            res = mulInt(res, i);
        }
        return res;
    }

    static BigInt mul1(const BigInt &a, u32 sm) { return mulInt(a, sm); }

    static BigInt mul10(BigInt a, i32 k) {
        if (k < 0) throw runtime_error("negative decimal shift");
        if (a.ask0() || !k) return a;
        i32 q = k / D, r = k % D;
        u32 p = 1;
        for (i32 i = 0; i < r; ++i) p *= 10;
        if (r) a = mulInt(a, p);
        if (q) a.d.insert(a.d.begin(), q, 0);
        return a;
    }

    static BigInt div10(BigInt a, i32 k) {
        if (k < 0) throw runtime_error("negative decimal shift");
        if (a.ask0() || !k) return a;
        i32 q = k / D, r = k % D;
        u32 p = 1;
        for (i32 i = 0; i < r; ++i) p *= 10;
        if (r) a = divmod(a, p).first;
        i32 n = a.d.size();
        if (q >= n) return BigInt(0);
        if (q) a.d.erase(a.d.begin(), a.d.begin() + q);
        a.norm();
        return a;
    }

    friend ostream &operator<<(ostream &os, const BigInt &x) {
        os << x.str();
        return os;
    }
    friend istream &operator>>(istream &is, BigInt &x) {
        string s;
        is >> s;
        x.read(s);
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
