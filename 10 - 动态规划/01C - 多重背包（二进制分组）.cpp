/*
用途：多重背包的二进制分组实现；代码短、适合物品数量不太大。
复杂度：O(m * sum(log(c+1)))。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T> struct Item {
    int w, c;
    T v;
};

template <class T>
vector<T> packMul(int m, const vector<Item<T>>& a) {
    assert(m >= 0);
    vector<T> f(m + 1);
    for (auto [w, c, v] : a) {
        assert(w > 0 && c >= 0);
        for (int k = 1; c > 0; k <<= 1) {
            int x = min<int>(k, c);
            c -= x;
            int sw = w * x;
            if (sw > m) continue;
            T sv = v * T(x);
            for (int j = m; j >= sw; --j) f[j] = max(f[j], f[j - sw] + sv);
        }
    }
    return f;
}
