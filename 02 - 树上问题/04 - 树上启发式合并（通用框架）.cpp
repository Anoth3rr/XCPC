/*
用途：DSU on Tree / Sack 通用框架。

适用：对每个子树统计颜色、值域、出现次数等可“加入/删除一个点”的信息。

接口：
- add(u, v)：加入无向边。
- work(addVertex, answer, root = 1)：
  * addVertex(vertex, delta)：delta 为 +1 或 -1，维护当前数据结构；
  * answer(u)：此时当前数据结构恰为 u 的子树，记录答案。

内部维护 tin / tout / euler / siz / heavy，子树区间为 [tin[u], tout[u]]。
复杂度：每个点至多被加入/删除 O(log n) 次，总计 O(n log n) 次回调。
*/
struct DSUOnTree {
    int n, tim;
    vector<vector<int>> e;
    vector<int> fa, siz, son, tin, out, ord;

    explicit DSUOnTree(int n) : n(n), tim(0), e(n + 1), fa(n + 1), siz(n + 1),
                                son(n + 1), tin(n + 1), out(n + 1), ord(n + 1) {}

    void add(int u, int v) {
        e[u].push_back(v);
        e[v].push_back(u);
    }

    void dfsSz(int u, int _fa) {
        fa[u] = _fa;
        siz[u] = 1;
        tin[u] = ++tim;
        ord[tim] = u;
        for (int v : e[u]) {
            if (v == _fa) continue;
            dfsSz(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[son[u]]) son[u] = v;
        }
        out[u] = tim;
    }

    template <class Add>
    void addSub(int u, int d, Add &add) const {
        for (int i = tin[u]; i <= out[u]; ++i) add(ord[i], d);
    }

    template <class A, class F>
    void solve(int u, bool kp, A &add, F &ans) {
        for (int v : e[u]) {
            if (v == fa[u] || v == son[u]) continue;
            solve(v, false, add, ans);
        }
        if (son[u]) solve(son[u], true, add, ans);

        for (int v : e[u]) {
            if (v == fa[u] || v == son[u]) continue;
            addSub(v, +1, add);
        }
        add(u, +1);
        ans(u);

        if (!kp) addSub(u, -1, add);
    }

    template <class A, class F>
    void work(A add, F ans, int rt = 1) {
        tim = 0;
        fill(fa.begin(), fa.end(), 0);
        fill(siz.begin(), siz.end(), 0);
        fill(son.begin(), son.end(), 0);
        fill(tin.begin(), tin.end(), 0);
        fill(out.begin(), out.end(), 0);
        dfsSz(rt, 0);
        solve(rt, true, add, ans);
    }
};
