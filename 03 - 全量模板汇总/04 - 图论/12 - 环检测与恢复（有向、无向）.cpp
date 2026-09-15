/*
用途：在线性时间内检测并恢复任意一个有向或无向环。
约定：点编号 1..n；FindCycle<true> 为有向图，FindCycle<false> 为无向图，均支持自环和重边。
接口：add(u,v) 后 ask() 返回环上的点，按环顺序且不重复首点；无环返回空数组。
复杂度：O(n+m) 时间、O(n+m) 空间。
*/
template <bool Dir>
struct FindCycle {
    int n, ed = 0;
    vector<vector<pii>> g;
    vector<int> col, pos, st;

    explicit FindCycle(int n) : n(n), g(n + 1) {}

    void add(int u, int v) {
        assert(1 <= u && u <= n && 1 <= v && v <= n);
        g[u].push_back({v, ed});
        if constexpr (!Dir) g[v].push_back({u, ed});
        ++ed;
    }

    vector<int> ask() {
        col.assign(n + 1, 0);
        pos.assign(n + 1, -1);
        st.clear();
        auto dfs = [&](auto&& go, int u, int pe) -> bool {
            col[u] = 1;
            pos[u] = st.size();
            st.push_back(u);
            for (auto [v, id] : g[u]) {
                if constexpr (!Dir) {
                    if (id == pe) continue;
                }
                if (col[v] == 0) {
                    if (go(go, v, id)) return true;
                } else if (col[v] == 1) {
                    vector<int> res(st.begin() + pos[v], st.end());
                    st = move(res);
                    return true;
                }
            }
            col[u] = 2;
            st.pop_back();
            return false;
        };
        for (int i = 1; i <= n; ++i) {
            if (col[i] == 0 && dfs(dfs, i, -1)) return st;
        }
        return {};
    }
};
