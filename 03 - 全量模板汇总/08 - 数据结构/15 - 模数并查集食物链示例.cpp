int solve() {
    ModDSU dsu(n, 3);
    int wrong = 0;
    while (k--) {
        int op, x, y;
        cin >> op >> x >> y;
        wrong += !dsu.add(x, y, op - 1);
    }
    return wrong;
}
