const ll MAXN = 5e5 + 5; // 原图最大节点数

class VBCC_Tree {
private:
    vector<ll> G[MAXN];        // 原图
    vector<ll> BCT[MAXN * 2];  // 圆方树（包含圆点和方点，空间开2倍）

    ll dfn[MAXN], low[MAXN], timer;
    ll stk[MAXN], top;
    ll n, bct_tot; 
    bool cut[MAXN]; // 记录哪些点是割点

public:
    void init(ll _n) {
        n = _n;
        bct_tot = n; // 1~n 是圆点，从 n+1 开始分配方点
        timer = top = 0;
        for (ll i = 1; i <= n * 2; i++) {
            G[i].clear();
            BCT[i].clear();
            dfn[i] = low[i] = 0;
            cut[i] = false;
        }
    }

    void add_edge(ll u, ll v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }

    // 在圆方树中加边
    void add_bct_edge(ll u, ll v) {
        BCT[u].push_back(v);
        BCT[v].push_back(u);
    }

    void tarjan(ll u, ll fa = 0) {
        dfn[u] = low[u] = ++timer;
        stk[++top] = u;
        ll child_cnt = 0; // 记录搜索树上的子节点数量（用于判断根是否为割点）
        
        for (ll v : G[u]) {
            if (v == fa) continue;
            if (!dfn[v]) {
                child_cnt++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                
                // 发现一个点双连通分量
                if (low[v] >= dfn[u]) {
                    cut[u] = true; // u 是割点
                    bct_tot++;     // 新建一个方点代表该点双
                    
                    // 当前点 u 连向方点 (注意 u 此时不出栈)
                    add_bct_edge(u, bct_tot);
                    
                    // 将点双内的点依次弹栈并连向方点
                    ll z;
                    do {
                        z = stk[top--];
                        add_bct_edge(z, bct_tot);
                    } while (z != v); // 注意这里弹到 v 为止，u 保留在栈中
                }
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        // 特判根节点是否为割点（根节点如果有>=2个独立分支才是割点）
        if (fa == 0 && child_cnt < 2) cut[u] = false;
    }

    void build() {
        for (ll i = 1; i <= n; i++) {
            if (!dfn[i]) {
                top = 0; 
                tarjan(i, 0);
            }
        }
    }
};