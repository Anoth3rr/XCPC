signed main() {
    int n;
    cin >> n;
    vector<int> w(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
    }

    int q;
    cin >> q;
    vector<array<int, 3>> query(q + 1);
    for (int i = 1; i <= q; i++) {
        int l, r;
        cin >> l >> r;
        query[i] = {l, r, i};
    }

    int Knum = n / min<int>(n, sqrt(q)); // 计算块长
    vector<int> K(n + 1);
    for (int i = 1; i <= n; i++) { // 固定块长
        K[i] = (i - 1) / Knum + 1;
    }
    sort(query.begin() + 1, query.end(), [&](auto x, auto y) {
        if (K[x[0]] != K[y[0]]) return x[0] < y[0];
        if (K[x[0]] & 1) return x[1] < y[1];
        return x[1] > y[1];
    });

    int l = 1, r = 0, val = 0;
    vector<int> ans(q + 1);
    for (int i = 1; i <= q; i++) {
        auto [ql, qr, id] = query[i];
        auto add = [&](int x) -> void {};
        auto del = [&](int x) -> void {};
        while (l > ql) add(w[--l]);
        while (r < qr) add(w[++r]);
        while (l < ql) del(w[l++]);
        while (r > qr) del(w[r--]);
        ans[id] = val;
    }
    for (int i = 1; i <= q; i++) {
        cout << ans[i] << endl;
    }
}
