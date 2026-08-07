/*
用途：无权图的 BFS/迭代 DFS；另附无向图二分图染色。
约定：点编号 1..n。Graph 可按需加有向边或无向边；
Bipartite 只应加入无向边。BFS 的 dis 为 -1 表示不可达。
复杂度：每次遍历或染色 O(n + m)。
*/
struct Graph {
    struct BFSResult {
        vector<int> dis;
        vector<int> fa;
        vector<int> ord;
    };

    int n;
    vector<vector<int>> g;

    explicit Graph(int n) : n(n), g(n + 1) {}

    void add(int u, int v) {
        g[u].push_back(v);
    }

    void add2(int u, int v) {
        add(u, v);
        add(v, u);
    }

    BFSResult bfs(int s) const {
        BFSResult res{vector<int>(n + 1, -1), vector<int>(n + 1, -1), {}};
        queue<int> q;
        res.dis[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            res.ord.push_back(u);
            for (int v : g[u]) {
                if (res.dis[v] != -1) continue;
                res.dis[v] = res.dis[u] + 1;
                res.fa[v] = u;
                q.push(v);
            }
        }
        return res;
    }

    // 显式栈版先序 DFS，避免链状图递归爆栈。
    vector<int> dfs(int s) const {
        vector<char> vis(n + 1, false);
        vector<int> it(n + 1), ord;
        vector<int> stk = {s};
        vis[s] = true;
        ord.push_back(s);
        while (!stk.empty()) {
            int u = stk.back();
            if (it[u] == g[u].size()) {
                stk.pop_back();
                continue;
            }
            int v = g[u][it[u]++];
            if (vis[v]) continue;
            vis[v] = true;
            ord.push_back(v);
            stk.push_back(v);
        }
        return ord;
    }
};

struct Bipartite {
    int n;
    vector<vector<int>> g;

    explicit Bipartite(int n) : n(n), g(n + 1) {}

    void add(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // color[u] 为 0/1；返回 nullopt 表示含奇环。
    optional<vector<int>> solve() const {
        vector<int> col(n + 1, -1);
        for (int s = 1; s <= n; ++s) {
            if (col[s] != -1) continue;
            queue<int> q;
            q.push(s);
            col[s] = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int v : g[u]) {
                    if (col[v] == -1) {
                        col[v] = col[u] ^ 1;
                        q.push(v);
                    } else if (col[v] == col[u]) {
                        return nullopt;
                    }
                }
            }
        }
        return col;
    }
};
