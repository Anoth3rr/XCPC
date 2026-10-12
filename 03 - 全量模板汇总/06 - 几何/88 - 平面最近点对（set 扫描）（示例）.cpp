i64 closestPair(vector<P> a) {
    assert(a.size() >= 2);
    sort(a.begin(), a.end(), [](P a, P b) { return tie(a.x, a.y) < tie(b.x, b.y); });
    auto dis = [](P a, P b) { return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y); };
    for (int i = 1; i < a.size(); ++i)
        if (a[i].x == a[i-1].x && a[i].y == a[i-1].y) return 0;
    i64 ans = dis(a[0], a[1]);
    set<tuple<i64, i64, int>> st; // y, x, 编号
    for (int i = 0, j = 0; i < a.size(); ++i) {
        while (j < i && (a[i].x-a[j].x)*(a[i].x-a[j].x) > ans) {
            st.erase({a[j].y, a[j].x, j});
            ++j;
        }
        i64 d = sqrtl(ans);
        while (d + 1 <= ans / (d + 1)) ++d;
        while (d && d > ans / d) --d;
        auto it = st.lower_bound({a[i].y-d, numeric_limits<i64>::lowest(), -1});
        for (; it != st.end() && get<0>(*it) <= a[i].y+d; ++it) {
            auto [y, x, id] = *it;
            ans = min(ans, dis(a[i], {x, y}));
        }
        st.insert({a[i].y, a[i].x, i});
    }
    return ans;
}
