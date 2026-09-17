/*
用途：计数无向或有向简单图中长度至少 3 的本质不同简单环。
约定：点编号 0..n-1；CycleCnt<false> 为无向图，CycleCnt<true> 为有向图；不支持重边和自环。
接口：add(u,v)，ask() 返回环数。无向环不区分方向，有向环区分方向。
复杂度：O(n*2^n) 时间、O(n*2^n) 空间；默认 int 使 n=20 约需 160 MB。
*/
template <bool Dir = false>
struct CycleCnt {
    using u64 = unsigned long long;

    int n;
    vector<u64> g;

    explicit CycleCnt(int n) : n(n), g(n) {
        assert(1 <= n && n <= 20);
    }

    void add(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        g[u] |= u64(1) << v;
        if constexpr (!Dir) g[v] |= u64(1) << u;
    }

    int ask() const {
        int m = 1 << n;
        u64 all = (u64(1) << n) - 1;
        vector<int> f(m * n);
        for (int s = 0; s < n; ++s) f[(1 << s) * n + s] = 1;
        int ans = 0;
        for (int s = 1; s < m; ++s) {
            int p = __builtin_ctz(static_cast<unsigned>(s));
            int pc = __builtin_popcount(static_cast<unsigned>(s));
            u64 ok = all ^ ((u64(1) << (p + 1)) - 1);
            for (int v = 0; v < n; ++v) {
                int cur = f[s * n + v];
                if (!cur) continue;
                if (v != p && pc >= 3 && (g[v] >> p & 1)) ans += cur;
                u64 can = g[v] & (all ^ u64(s)) & ok;
                while (can) {
                    int u = __builtin_ctzll(can);
                    f[(s | (1 << u)) * n + u] += cur;
                    can &= can - 1;
                }
            }
        }
        if constexpr (!Dir) ans /= 2;
        return ans;
    }
};
