const ll N = 1e5+9;
const ll M = 2e6+9+1e5;
ll dep[N], head[M], to[M], from[M], w[M], nxt[M], cur[N], S, T, ecnt = 0;
void adde(ll x, ll y, ll z) {
    from[ecnt] = x; to[ecnt] = y; w[ecnt] = z; 
    nxt[ecnt] = head[x]; head[x] = ecnt++;
}
void addes(ll x, ll y, ll z) { adde(x, y, z); adde(y, x, 0); }

bool bfs() {
    static ll q[N]; // 静态数组模拟队列
    ll front = 0, rear = 0;
    memset(dep, -1, sizeof dep);
    memcpy(cur, head, sizeof cur);
    dep[S] = 1;
    q[rear++] = S;
    while (front < rear) {
        ll u = q[front++];
        for (ll i = head[u]; ~i; i = nxt[i]) {
            ll v = to[i];
            if (dep[v] == -1 && w[i] > 0) {
                dep[v] = dep[u] + 1;
                if (v == T) return true; // 提前终止
                q[rear++] = v;
            }
        }
    }
    return dep[T] != -1;
}

ll dfs(ll u, ll low) {
    if (u == T) return low;
    ll ret = low;
    for (ll& i = cur[u]; ~i; i = nxt[i]) { // 当前弧优化，使用引用
        ll v = to[i];
        if (dep[v] == dep[u] + 1 && w[i] > 0) {
            ll flow = dfs(v, min(ret, w[i]));
            if (flow > 0) {
                w[i] -= flow;
                w[i^1] += flow;
                ret -= flow;
                if (ret == 0) break;
            } else {
                dep[v] = -1; // 剪枝优化，无法到达T的点不再访问
            }
        }
    }
    return low - ret;
}

ll dinic() {
    ll max_flow = 0;
    while (bfs()) max_flow += dfs(S, inf);
    return max_flow;
}

void init() { // 重置
    ecnt = 0;
    memset(head, -1, sizeof head);
}