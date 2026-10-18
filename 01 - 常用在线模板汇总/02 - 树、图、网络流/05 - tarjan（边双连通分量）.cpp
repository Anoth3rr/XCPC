const ll MAXN = 5e5 + 5; 
const ll MAXM = 1e6 + 5; // 最大边数

struct Edge {
    ll to; // 目标点
    ll id; // 边编号
};

class BridgeFinder {
private:
    vector<Edge> G[MAXN];
    ll dfn[MAXN], low[MAXN], timer;
    ll edge_tot;
    ll n;

public:
    bool is_bridge[MAXM];         // is_bridge[i] 表示第 i 条输入的边是否是割边
    vector<pair<ll, ll>> bridges; // 收集所有的割边对 (u, v)

    void init(ll _n) {
        n = _n;
        timer = edge_tot = 0;
        bridges.clear();
        for (ll i = 1; i <= n; i++) {
            G[i].clear();
            dfn[i] = low[i] = 0;
        }
    }

    // 添加无向边 (两端点共用同一个边编号)
    void add_edge(ll u, ll v) {
        edge_tot++;
        is_bridge[edge_tot] = false;
        G[u].push_back({v, edge_tot});
        G[v].push_back({u, edge_tot});
    }

    // in_edge_id 是从父亲走过来的那条边的编号
    void tarjan(ll u, ll in_edge_id = 0) {
        dfn[u] = low[u] = ++timer;

        for (auto& edge : G[u]) {
            ll v = edge.to;
            ll id = edge.id;
            
            if (id == in_edge_id) continue;

            if (!dfn[v]) {
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > dfn[u]) {
                    is_bridge[id] = true;
                    bridges.push_back({min(u, v), max(u, v)});
                }
            } 
            else {
                low[u] = min(low[u], dfn[v]);
            }
        }
    }

    // 执行求解
    void solve() {
        for (ll i = 1; i <= n; i++) {
            if (!dfn[i]) {
                tarjan(i, 0); // 初始调用的入边编号为 0
            }
        }
    }
};
