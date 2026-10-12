int intersection(int x, int y, int X, int Y) {
    vector<int> t = {lca(x, X), lca(x, Y), lca(y, X), lca(y, Y)};
    sort(t.begin(), t.end(), [&](int a, int b) { return dep[a] < dep[b]; });
    int r = lca(x, y), R = lca(X, Y);
    if (dep[t[0]] < min(dep[r], dep[R]) || dep[t[2]] < max(dep[r], dep[R])) {
        return 0;
    }
    return 1 + clac(t[2], t[3]);
}
