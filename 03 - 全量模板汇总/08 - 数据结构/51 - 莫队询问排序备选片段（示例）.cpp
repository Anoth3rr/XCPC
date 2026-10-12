vector<array<int, 4>> query(q);
for (int i = 1; i <= q; i++) {
    int l, r;
    cin >> l >> r;
    query[i] = {l, r, i, (l - 1) / Knum + 1}; // 合并
}
sort(query.begin() + 1, query.end(), [&](auto x, auto y) {
    if (x[3] != y[3]) return x[3] < y[3];
    if (x[3] & 1) return x[1] < y[1];
    return x[1] > y[1];
});
