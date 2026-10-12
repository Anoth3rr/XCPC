const int N = 1e6 + 10;
int T, n, a[N], c, tr[N << 2];
void pushup(int u){
    tr[u] = max(tr[u << 1], tr[u << 1 | 1]);
}
void build(int u, int l, int r){
    if (l == r) tr[u] = c;
    else {
        int mid = l + r >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
}
void update(int u, int l, int r, int p, int k){
    if (l > p || r < p) return;
    if (l == r) tr[u] -= k;
    else {
        int mid = l + r >> 1;
        update(u << 1, l, mid, p, k);
        update(u << 1 | 1, mid + 1, r, p, k);
        pushup(u);
    }
}
int query(int u, int l, int r, int k){
    if (l == r){
        if (tr[u] >= k) return l;
        return n + 1;
    }
    int mid = l + r >> 1;
    if (tr[u << 1] >= k) return query(u << 1, l, mid, k);
    else return query(u << 1 | 1, mid + 1, r, k);
}
int main() {
    cin >> n >> c;
    for (int i = 1; i <= n; i++) cin >> a[i];
    build(1, 1, n);
    for (int i = 1; i <= n; i++)
        update(1, 1, n, query(1, 1, n, a[i]), a[i]);
    cout << query(1, 1, n, c) - 1 << " ";
}
