template <class T> struct Rerooting {
    struct Edge {
        int to, id;
    };
    using Merge = function<T(const T &, const T &)>;
    using Transfer = function<T(const T &, int, int, int)>;

    int n, m = 0;
    T id;
    Merge mg;
    Transfer trn;
    vector<vector<Edge>> e;
    vector<int> fa, pe, ord;
    vector<T> bas, dn, up, ans;

    Rerooting(int n, T id, Merge mg, Transfer trn) : n(n), id(id), mg(move(mg)), trn(move(trn)), e(n + 1), fa(n + 1), pe(n + 1), bas(n + 1, id), dn(n + 1, id), up(n + 1, id), ans(n + 1, id) {}

    int add(int u, int v) {
        int id = m++;
        e[u].push_back({v, id});
        e[v].push_back({u, id});
        return id;
    }

    void work(int rt = 1) {
        fill(fa.begin(), fa.end(), 0);
        fill(pe.begin(), pe.end(), -1);
        ord.clear();
        ord.push_back(rt);
        for (int i = 0; i < ord.size(); ++i) {
            int u = ord[i];
            for (auto [v, id] : e[u]) {
                if (v == fa[u]) continue;
                fa[v] = u;
                pe[v] = id;
                ord.push_back(v);
            }
        }

        fill(dn.begin(), dn.end(), id);
        for (int i = ord.size(); i--;) {
            int u = ord[i];
            dn[u] = bas[u];
            for (auto [v, id] : e[u]) {
                if (fa[v] != u) continue;
                dn[u] = mg(dn[u], trn(dn[v], v, u, id));
            }
        }

        fill(up.begin(), up.end(), id);
        for (int u : ord) {
            int deg = e[u].size();
            vector<T> get(deg), pre(deg + 1, id), suf(deg + 1, id);
            for (int i = 0; i < deg; ++i) {
                auto [v, id] = e[u][i];
                get[i] = (v == fa[u]) ? up[u] : trn(dn[v], v, u, id);
                pre[i + 1] = mg(pre[i], get[i]);
            }
            for (int i = deg - 1; i >= 0; --i) suf[i] = mg(get[i], suf[i + 1]);
            ans[u] = mg(bas[u], pre[deg]);

            for (int i = 0; i < deg; ++i) {
                auto [v, id] = e[u][i];
                if (fa[v] != u) continue;
                T oth = mg(bas[u], mg(pre[i], suf[i + 1]));
                up[v] = trn(oth, u, v, id);
            }
        }
    }
};