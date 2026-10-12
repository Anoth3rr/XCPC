struct Node {
    int v;
    Node(int x = 0) : v(norm(x)) {}
    int norm(int x) {
        x %= 3;
        return x < 0 ? x + 3 : x;
    }
    Node operator+(const Node o) { return norm(v + o.v); }
    Node operator-(const Node o) { return norm(v - o.v); }
    bool operator==(const Node o) { return norm(v) == norm(o.v); }
};
void solve() {
    wDSU<Node> tr(n);
    while (q--) {
        int op, x, y;
        cin >> op >> x >> y;
        if (x > n || y > n) {
            ans++;
            continue;
        }
        ans += !tr.add(x, y, op - 1);
    }
}
