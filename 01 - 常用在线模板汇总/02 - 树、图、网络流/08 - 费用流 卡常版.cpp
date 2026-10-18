const int V = 6e5 + 5, E = 3e7 + 5, inf = 0x3f3f3f3f, N = 1000;
int s, t, v = 0, ecnt = 1, from[V], to[E], nxt[E], w[E], c[E], dis[V], cur[V];
bool vis[V];
void adde(int x, int y, int z, int cost) {
    to[++ecnt] = y, nxt[ecnt] = from[x], from[x] = ecnt, w[ecnt] = z, c[ecnt] = cost;
    to[++ecnt] = x, nxt[ecnt] = from[y], from[y] = ecnt, w[ecnt] = 0, c[ecnt] = -cost;
}

unordered_map<int, int> id;
int n, a[N], p[V];

bool spfa() {
    memset(dis, 0x3f, sizeof(dis));
    deque<int> q;
    dis[s] = 0, q.push_back(s), vis[s] = 1;
    
    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        vis[u] = 0;
        for (int e = from[u]; e; e = nxt[e]) {
            int v = to[e], cost = c[e];
            if (w[e] && dis[v] > dis[u] + cost) {
                dis[v] = dis[u] + cost;
                if (!vis[v]) {
                    vis[v] = 1;
                    !q.empty() && dis[v] < dis[q.front()] ? q.push_front(v) : q.push_back(v);
                }
            }
        }
    }
    return dis[t] != inf;
}

int dfs(int u, int flow, int &cost) {
    if (u == t) return flow;
    vis[u] = 1;
    int ans = 0;
    for (int &e = cur[u]; e; e = nxt[e]) {
        int v = to[e], cost_e = c[e];
        if (!vis[v] && w[e] && dis[v] == dis[u] + cost_e) {
            int tmp = dfs(v, min(flow, w[e]), cost);
            if (tmp) {
                w[e] -= tmp;
                w[e^1] += tmp;
                cost += tmp * cost_e;
                flow -= tmp;
                ans += tmp;
                if (!flow) break;
            }
        }
    }
    vis[u] = 0;
    return ans;
}

pair<int, int> MCMF() {
    int flow = 0, cost = 0;
    while (spfa()) {
        memcpy(cur, from, sizeof(cur));
        while (int f = dfs(s, inf, cost)) 
            flow += f;
    }
    return {flow, cost};
}

void init() {
    ecnt = 1;     
    memset(from, 0, sizeof(from));    // 0 表示邻接表结束, 所以首条 ecnt 应为 2
    v = 0;
    s = t = 0;
    memset(vis, 0, sizeof(vis));
    memset(dis, 0x3f, sizeof(dis));
    memset(cur, 0, sizeof(cur));
    n = 0;
    memset(a, 0, sizeof(a));
    memset(p, 0, sizeof(p));
    id.clear();
}