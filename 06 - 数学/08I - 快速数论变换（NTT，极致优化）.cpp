/*
用途：32 位 Montgomery + radix-4 NTT，接口与 08 的 NTT<P,G> 一致。
用法：auto c = NTT<998244353, 3>::mul(a, b)；卡常时替换 08。
限制：P 为小于 2^30 的奇质数，G 为原根，变换长度整除 P-1。
性能：模数值与变换缓存必须保留 u32，循环下标保留 i32；改用宏展开后的 int 会显著变慢并扩大内存。
*/

using i32 = int32_t;
using u32 = uint32_t;
using u64 = uint64_t;

template <u32 P, u32 G> struct NTT {
    struct Z {
        u32 x = 0;

        static constexpr u32 askR() {
            u32 r = P;
            for (i32 i = 0; i < 5; ++i) r *= 2 - P * r;
            return -r;
        }

        static constexpr u32 mr = askR();
        static constexpr u32 m2 = -u64(P) % P;

        static constexpr u32 red(u64 x) {
            return (x + u64(u32(x) * mr) * P) >> 32;
        }

        Z() = default;
        Z(int v) {
            v %= P;
            if (v < 0) v += P;
            x = red(u64(v) * m2);
        }

        static Z raw(u32 x) {
            Z a;
            a.x = x;
            return a;
        }

        u32 val() const {
            u32 v = red(x);
            return v >= P ? v - P : v;
        }

        Z pow(int n) const {
            Z a = *this, r = 1;
            while (n) {
                if (n & 1) r *= a;
                a *= a;
                n >>= 1;
            }
            return r;
        }

        Z inv() const {
            return pow(P - 2);
        }

        Z& operator+=(Z a) {
            x += a.x;
            if (x >= 2 * P) x -= 2 * P;
            return *this;
        }

        Z& operator-=(Z a) {
            x += 2 * P - a.x;
            if (x >= 2 * P) x -= 2 * P;
            return *this;
        }

        Z& operator*=(Z a) {
            x = red(u64(x) * a.x);
            return *this;
        }

        friend Z operator+(Z a, Z b) { return a += b; }
        friend Z operator-(Z a, Z b) { return a -= b; }
        friend Z operator*(Z a, Z b) { return a *= b; }
    };

    static_assert(P & 1);
    static_assert(P < (1u << 30));
    static_assert(u32(-NTT::Z::mr * P) == 1);

    static inline bool ok = false;
    static inline array<Z, 30> rt, ir, q2, iq2, q3, iq3;

    static void init() {
        if (ok) return;
        ok = true;
        i32 k = __builtin_ctz(P - 1);
        rt[k] = Z(G).pow((P - 1) >> k);
        ir[k] = rt[k].inv();
        for (i32 i = k; i--;) {
            rt[i] = rt[i + 1] * rt[i + 1];
            ir[i] = ir[i + 1] * ir[i + 1];
        }
        Z a = 1, b = 1;
        for (i32 i = 0; i <= k - 2; ++i) {
            q2[i] = rt[i + 2] * a;
            iq2[i] = ir[i + 2] * b;
            a *= ir[i + 2], b *= rt[i + 2];
        }
        a = b = 1;
        for (i32 i = 0; i <= k - 3; ++i) {
            q3[i] = rt[i + 3] * a;
            iq3[i] = ir[i + 3] * b;
            a *= ir[i + 3], b *= rt[i + 3];
        }
    }

    static void ntt(vector<Z>& a, bool iv) {
        i32 n = a.size();
        assert(n && !(n & (n - 1)));
        i32 h = __builtin_ctz(u32(n));
        assert(h <= __builtin_ctz(P - 1));
        init();

        if (!iv) {
            i32 len = 0;
            while (len < h) {
                if (h - len == 1) {
                    i32 p = 1 << (h - len - 1);
                    Z w = 1;
                    for (i32 s = 0; s < (1 << len); ++s) {
                        i32 o = s << (h - len);
                        for (i32 i = 0; i < p; ++i) {
                            Z x = a[o + i], y = a[o + i + p] * w;
                            a[o + i] = x + y;
                            a[o + i + p] = x - y;
                        }
                        w *= q2[__builtin_ctz(~u32(s))];
                    }
                    ++len;
                } else {
                    i32 p = 1 << (h - len - 2);
                    Z w = 1, im = rt[2];
                    for (i32 s = 0; s < (1 << len); ++s) {
                        Z w2 = w * w, w3 = w2 * w;
                        i32 o = s << (h - len);
                        for (i32 i = 0; i < p; ++i) {
                            Z a0 = a[o + i];
                            Z a1 = a[o + i + p] * w;
                            Z a2 = a[o + i + 2 * p] * w2;
                            Z a3 = a[o + i + 3 * p] * w3;
                            Z x = (a1 - a3) * im;
                            a[o + i] = a0 + a1 + a2 + a3;
                            a[o + i + p] = a0 - a1 + a2 - a3;
                            a[o + i + 2 * p] = a0 - a2 + x;
                            a[o + i + 3 * p] = a0 - a2 - x;
                        }
                        w *= q3[__builtin_ctz(~u32(s))];
                    }
                    len += 2;
                }
            }
        } else {
            i32 len = h;
            while (len) {
                if (len == 1) {
                    i32 p = 1 << (h - len);
                    Z w = 1;
                    for (i32 s = 0; s < (1 << (len - 1)); ++s) {
                        i32 o = s << (h - len + 1);
                        for (i32 i = 0; i < p; ++i) {
                            Z x = a[o + i], y = a[o + i + p];
                            a[o + i] = x + y;
                            a[o + i + p] = (x - y) * w;
                        }
                        w *= iq2[__builtin_ctz(~u32(s))];
                    }
                    --len;
                } else {
                    i32 p = 1 << (h - len);
                    Z w = 1, im = ir[2];
                    for (i32 s = 0; s < (1 << (len - 2)); ++s) {
                        Z w2 = w * w, w3 = w2 * w;
                        i32 o = s << (h - len + 2);
                        for (i32 i = 0; i < p; ++i) {
                            Z a0 = a[o + i];
                            Z a1 = a[o + i + p];
                            Z a2 = a[o + i + 2 * p];
                            Z a3 = a[o + i + 3 * p];
                            Z x = (a2 - a3) * im;
                            a[o + i] = a0 + a1 + a2 + a3;
                            a[o + i + p] = (a0 - a1 + x) * w;
                            a[o + i + 2 * p] = (a0 + a1 - a2 - a3) * w2;
                            a[o + i + 3 * p] = (a0 - a1 - x) * w3;
                        }
                        w *= iq3[__builtin_ctz(~u32(s))];
                    }
                    len -= 2;
                }
            }
            Z z = Z(n).inv();
            for (auto& x : a) x *= z;
        }
    }

    static u32 norm(int x) {
        x %= P;
        if (x < 0) x += P;
        return x;
    }

    static vector<u32> mul32(const vector<int>& a, const vector<int>& b) {
        if (a.empty() || b.empty()) return {};
        i32 an = a.size(), bn = b.size();
        i32 m = an + bn - 1;
        if (min(an, bn) <= 32) {
            vector<Z> z(m);
            for (i32 i = 0; i < an; ++i)
                for (i32 j = 0; j < bn; ++j) z[i + j] += Z(a[i]) * Z(b[j]);
            vector<u32> r(m);
            for (i32 i = 0; i < m; ++i) r[i] = z[i].val();
            return r;
        }

        i32 n = 1;
        while (n < m) n <<= 1;
        if (m - 2 <= n / 2) {
            vector<int> x(a.begin(), a.end() - 1), y(b.begin(), b.end() - 1);
            u32 al = norm(a.back()), bl = norm(b.back());
            auto r = mul32(x, y);
            r.resize(m);
            r[m - 1] = u64(al) * bl % P;
            for (i32 i = 0; i < an - 1; ++i)
                r[i + bn - 1] = (r[i + bn - 1] + u64(norm(x[i])) * bl) % P;
            for (i32 i = 0; i < bn - 1; ++i)
                r[i + an - 1] = (r[i + an - 1] + u64(norm(y[i])) * al) % P;
            return r;
        }

        vector<Z> x(n), y(n);
        for (i32 i = 0; i < an; ++i) x[i] = Z(a[i]);
        for (i32 i = 0; i < bn; ++i) y[i] = Z(b[i]);
        bool eq = a == b;
        ntt(x, false);
        if (eq) y = x;
        else ntt(y, false);
        for (i32 i = 0; i < n; ++i) x[i] *= y[i];
        ntt(x, true);
        vector<u32> r(m);
        for (i32 i = 0; i < m; ++i) r[i] = x[i].val();
        return r;
    }

    static vector<int> mul(const vector<int>& a, const vector<int>& b) {
        auto x = mul32(a, b);
        return vector<int>(x.begin(), x.end());
    }
};
