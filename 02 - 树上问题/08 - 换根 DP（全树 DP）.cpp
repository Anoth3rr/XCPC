/*
用途：通用换根 DP（Rerooting），在一次 O(n) 遍历中计算每个点作为根时的全树答案。

DP 约定：
- base[u] 是节点 u 自身的初始贡献；
- merge(a, b) 合并同一节点收到的两份贡献，必须满足结合律，identity 为其单位元；
- transfer(value, from, to, edgeId) 将以 from 为根的一侧贡献跨边传给 to。

例：全树距离和可令 T = pair<int, int> = {点数, 距离和}，
base[u] = {1, 0}，merge 为逐项相加，transfer({cnt, sum}) = {cnt, sum + cnt * w[edgeId]}。

接口：
- Rerooting(n, identity, merge, transfer)，add(u, v) 返回边编号。
- 设置 base[u] 后调用 work(root = 1)，答案写入 answer[u]。
*/
template <class T>
struct Rerooting {
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

    Rerooting(int n, T id, Merge mg, Transfer trn)
        : n(n), id(id), mg(move(mg)), trn(move(trn)), e(n + 1),
          fa(n + 1), pe(n + 1), bas(n + 1, id), dn(n + 1, id),
          up(n + 1, id), ans(n + 1, id) {}

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
