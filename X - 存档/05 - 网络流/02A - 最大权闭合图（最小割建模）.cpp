/*
用途：最大权闭合图。若选择 u 必须选择 v，则 add(u,v)。
依赖：01A - 最大流（Dinic）的 MaxFlow。
约定：点编号 0..n-1；modify 可对同一点多次调用。依赖边容量应大于所有可能答案。
复杂度：一次 Dinic 最大流。
*/

template <class Cap = int> struct MaxClosure {
    static constexpr Cap INF = numeric_limits<Cap>::max() / 4;

    struct Result {
        Cap val;
        vector<char> vis;
    };

    int n;
    int s, t;
    MaxFlow<Cap> mf;
    Cap sum = 0;

    explicit MaxClosure(int n) : n(n), s(n), t(n + 1), mf(n + 2) {}

    void modify(int u, Cap w) {
        assert(0 <= u && u < n);
        if (w > 0) {
            mf.add(s, u, w);
            sum += w;
        } else if (w < 0) {
            mf.add(u, t, -w);
        }
    }

    void add(int u, int v, Cap c = INF) {
        assert(0 <= u && u < n && 0 <= v && v < n && c >= 0);
        mf.add(u, v, c);
    }

    Result solve() {
        mf.reset();
        Cap cut = mf.flow(s, t);
        vector<char> sd = mf.askCut(s);
        vector<char> vis(n, false);
        for (int u = 0; u < n; ++u) vis[u] = sd[u];
        return {sum - cut, move(vis)};
    }
};
