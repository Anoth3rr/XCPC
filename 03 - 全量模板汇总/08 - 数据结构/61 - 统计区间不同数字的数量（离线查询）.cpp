signed main() {
    int n;
    cin >> n;
    vector<int> in(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> in[i];
    }
    
    int q;
    cin >> q;
    vector<array<int, 3>> query;
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        query.push_back({r, l, i});
    }
    sort(query.begin(), query.end());
    
    vector<pair<int, int>> ans;
    map<int, int> pre;
    int st = 1;
    BIT bit(n);
    for (auto [r, l, id] : query) {
        for (int i = st; i <= r; i++, st++) {
            if (pre.count(in[i])) { // 消除此前操作的影响
                bit.add(pre[in[i]], -1);
            }
            bit.add(i, 1);
            pre[in[i]] = i; // 更新操作
        }
        ans.push_back({id, bit.ask(r) - bit.ask(l - 1)});
    }
    
    sort(ans.begin(), ans.end());
    for (auto [id, w] : ans) {
        cout << w << endl;
    }
}
