/*
用途：无向带权图的最小生成森林；ok 为真时即最小生成树。
约定：点编号 1..n，可含重边和负边权。
复杂度：O(m log m)。
*/

struct MST {

    struct Edge {
        int u, v;
        int w;
    };

    struct Result {
        bool ok;
        int w;              // 非连通时为最小生成森林的权值。
        vector<Edge> vis;  // 非连通时为最小生成森林的边。
    };

    struct DSU {
        vector<int> fa, sz;

        explicit DSU(int n) : fa(n + 1), sz(n + 1, 1) {
            iota(fa.begin(), fa.end(), 0);
        }

        int find(int x) {
            while (x != fa[x]) {
                x = fa[x] = fa[fa[x]];
            }
            return x;
        }

        bool merge(int x, int y) {
            x = find(x);
            y = find(y);
            if (x == y) return false;
            if (sz[x] < sz[y]) swap(x, y);
            fa[y] = x;
            sz[x] += sz[y];
            return true;
        }
    };

    int n;
    vector<Edge> e;

    explicit MST(int n) : n(n) {}

    void add(int u, int v, int w) {
        e.push_back({u, v, w});
    }

    Result solve() const {
        vector<Edge> ord = e;
        sort(ord.begin(), ord.end(), [](const Edge& a, const Edge& b) {
            return a.w < b.w;
        });

        DSU d(n);
        int sum = 0;
        vector<Edge> vis;
        for (const Edge& x : ord) {
            if (!d.merge(x.u, x.v)) continue;
            sum += x.w;
            vis.push_back(x);
        }
        return {vis.size() == n - 1, sum, move(vis)};
    }
};
