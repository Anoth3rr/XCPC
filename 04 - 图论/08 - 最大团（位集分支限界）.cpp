/*
用途：求 n<=64 无向简单图的最大团及一组方案。
复杂度：最坏指数级；贪心染色给上界，适合中小规模稠密图。
性能：邻接与候选集使用 u64，避免 #define int long long 扩大状态。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

struct Clique {
    int n;
    vector<u64> g;
    vector<int> now, ans;

    explicit Clique(int _n) : n(_n), g(_n) { assert(0 <= n && n <= 64); }
    void add(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n && u != v);
        g[u] |= u64{1} << v;
        g[v] |= u64{1} << u;
    }

    vector<int> solve() {
        now.clear(), ans.clear();
        dfs(n == 64 ? ~u64{} : (u64{1} << n) - 1);
        return ans;
    }

  private:
    void color(u64 p, vector<int>& v, vector<int>& c) {
        for (int k = 1; p; ++k) {
            u64 q = p;
            while (q) {
                int x = __builtin_ctzll(q);
                u64 b = u64{1} << x;
                v.push_back(x), c.push_back(k);
                p ^= b, q ^= b, q &= ~g[x];
            }
        }
    }

    void dfs(u64 p) {
        if (!p) {
            if (now.size() > ans.size()) ans = now;
            return;
        }
        vector<int> v, c;
        color(p, v, c);
        for (int i = v.size(); i--;) {
            if (now.size() + c[i] <= ans.size()) return;
            int x = v[i];
            now.push_back(x), dfs(p & g[x]), now.pop_back();
            p &= ~(u64{1} << x);
        }
    }
};
