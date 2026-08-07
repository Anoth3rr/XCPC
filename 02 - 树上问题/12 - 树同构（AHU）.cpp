/*
用途：无根树同构判定（AHU canonical form，无随机哈希碰撞）。

输入邻接表必须为 1-indexed：g.size() == n + 1，g[1..n] 构成一棵树。
ask(g,h) 返回两棵无标号树是否同构，复杂度 O(n log n)（子树类型排序与 map）。
若题目要求保留点权/颜色，可把它们加入 rootId 的 key 中。
*/
struct TreeIso {
    static vector<int> centers(const vector<vector<int>> &g) {
        int n = g.size() - 1;
        if (n == 1) return {1};

        vector<int> deg(n + 1);
        vector<char> ban(n + 1);
        queue<int> q;
        for (int u = 1; u <= n; ++u) {
            deg[u] = g[u].size();
            if (deg[u] <= 1) q.push(u);
        }

        int rem = n;
        while (rem > 2) {
            int cnt = q.size();
            rem -= cnt;
            while (cnt--) {
                int u = q.front();
                q.pop();
                ban[u] = true;
                for (int v : g[u]) {
                    if (!ban[v] && --deg[v] == 1) q.push(v);
                }
            }
        }

        vector<int> res;
        for (int u = 1; u <= n; ++u) {
            if (!ban[u]) res.push_back(u);
        }
        return res;
    }

    static int rootId(const vector<vector<int>> &g, int u, int fa, map<vector<int>, int> &mp) {
        vector<int> key;
        for (int v : g[u]) {
            if (v != fa) key.push_back(rootId(g, v, u, mp));
        }
        sort(all(key));
        auto it = mp.find(key);
        if (it == mp.end()) {
            int id = mp.size() + 1;
            it = mp.emplace(move(key), id).first;
        }
        return it->second;
    }

    static bool ask(const vector<vector<int>> &g, const vector<vector<int>> &h) {
        if (g.size() != h.size()) return false;
        map<vector<int>, int> mp;
        set<int> id;
        for (int rt : centers(g)) id.insert(rootId(g, rt, 0, mp));
        for (int rt : centers(h)) {
            if (id.count(rootId(h, rt, 0, mp))) return true;
        }
        return false;
    }
};
