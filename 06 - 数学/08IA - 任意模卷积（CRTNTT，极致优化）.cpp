/*
用途：三次极低常数 NTT + CRT 的精确任意模卷积，可过针对浮点拆分的 FFT killer。
依赖：08I - 快速数论变换（NTT，极致优化）。
用法：CRTNTT f；auto c = f.mul(a, b, p)。输入会先规范到 [0,p)。
限制：p <= 2^32-1，变换长度不超过 2^21；三模乘积必须大于单项卷积的绝对上界。
性能：三份余数缓存必须保留 u32；改用宏展开后的 int 会额外占用大量内存并降低缓存命中率。
*/

using u32 = uint32_t;
using u64 = uint64_t;

struct CRTNTT {
    static constexpr u32 p1 = 998244353;
    static constexpr u32 p2 = 1004535809;
    static constexpr u32 p3 = 104857601;
    static constexpr u32 i1 = 669690699;
    static constexpr u32 i2 = 99747662;

    static int norm(int x, int p) {
        x %= p;
        if (x < 0) x += p;
        return x;
    }

    vector<int> mul(const vector<int>& x, const vector<int>& y, int p) const {
        if (x.empty() || y.empty()) return {};
        assert(0 < p && p <= 4294967295LL);
        int nx = x.size(), ny = y.size();
        assert(nx + ny - 1 <= (1LL << 21));
        vector<int> a(nx), b(ny);
        for (int i = 0; i < nx; ++i) a[i] = norm(x[i], p);
        for (int i = 0; i < ny; ++i) b[i] = norm(y[i], p);

        auto a1 = NTT<p1, 3>::mul32(a, b);
        auto a2 = NTT<p2, 3>::mul32(a, b);
        auto a3 = NTT<p3, 3>::mul32(a, b);
        vector<int> r(a1.size());
        int n = r.size();
        u64 m1 = p1 % p;
        u64 m2 = m1 * (p2 % p) % p;
        for (int i = 0; i < n; ++i) {
            u64 x1 = a1[i];
            u64 x2 = (u64(a2[i]) + p2 - x1 % p2) * i1 % p2;
            u64 x3 = (u64(a3[i]) + p3 - (x1 + x2 * p1) % p3) * i2 % p3;
            r[i] = (x1 % p + x2 % p * m1 + x3 % p * m2) % p;
        }
        return r;
    }
};
