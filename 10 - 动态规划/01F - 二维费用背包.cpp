/*
用途：每件物品至多取一次，同时受两维容量约束。
item = {第一维费用, 第二维费用, 价值}。
复杂度：O(nab) 时间、O(ab) 空间。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T> struct Item2D {
    int x, y;
    T v;
};

template <class T>
vector<vector<T>> pack2D(int a, int b, const vector<Item2D<T>>& c) {
    assert(a >= 0 && b >= 0);
    vector f(a + 1, vector<T>(b + 1));
    for (auto [x, y, v] : c) {
        assert(x > 0 || y > 0);
        for (int i = a; i >= x; --i)
            for (int j = b; j >= y; --j) f[i][j] = max(f[i][j], f[i - x][j - y] + v);
    }
    return f;
}
