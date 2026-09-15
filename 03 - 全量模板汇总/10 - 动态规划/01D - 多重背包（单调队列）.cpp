/*
用途：多重背包的单调队列实现；物品数量 c 很大时不再多一个 log c。
复杂度：O(nm) 时间、O(m) 空间。
与 01C 同名、同接口，二者择一使用。
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
        vector<T> g = f;
        for (int r = 0; r < w && r <= m; ++r) {
            deque<int> q;
            int z = (m - r) / w;
            for (int k = 0; k <= z; ++k) {
                while (!q.empty() && q.front() < k - c) q.pop_front();
                T x = g[r + k * w] - T(k) * v;
                while (!q.empty() && g[r + q.back() * w] - T(q.back()) * v <= x) q.pop_back();
                q.push_back(k);
                int p = q.front();
                f[r + k * w] = g[r + p * w] + T(k - p) * v;
            }
        }
    }
    return f;
}
