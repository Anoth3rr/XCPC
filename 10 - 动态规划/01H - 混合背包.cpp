/*
用途：同一组物品混合 01、完全、多重三种数量限制，求容量不超过 m 的最大价值。
约定：Item.c=-1 表示至多一次，0 表示无限次，正数表示至多 c 次；w>0。
复杂度：二进制拆分多重物品，O(m * 拆分后的物品数)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T> struct MixedItem {
    int w, c;
    T v;
};

template <class T> vector<T> packMix(int m, const vector<MixedItem<T>>& a) {
    assert(m >= 0);
    vector<T> f(m + 1, T{});
    auto one = [&](int w, T v) {
        for (int j = m; j >= w; --j) f[j] = max(f[j], f[j - w] + v);
    };
    for (auto [w, c, v] : a) {
        assert(w > 0 && c >= -1);
        if (c < 0) one(w, v);
        else if (!c) for (int j = w; j <= m; ++j) f[j] = max(f[j], f[j - w] + v);
        else {
            for (int k = 1; c;) {
                int x = min(k, c);
                one(w * x, v * x);
                c -= x;
                k = k <= c ? k << 1 : c;
            }
        }
    }
    return f;
}
