/*
用途：稠密二维树状数组，支持单点加与矩形和。
约定：坐标均为 1-indexed；ask(x1,y1,x2,y2) 为闭矩形。
复杂度：修改/查询 O(log n log m)，空间 O(nm)。坐标很大且操作稀疏时应离线压缩或改动态结构。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T> struct BIT {
    int n = 0, m = 0;
    vector<vector<T>> t;

    BIT() = default;
    BIT(int n, int m) { init(n, m); }

    void init(int _n, int _m) {
        assert(_n >= 0 && _m >= 0);
        n = _n, m = _m;
        t.assign(n + 1, vector<T>(m + 1));
    }

    void modify(int x, int y, const T& v) {
        assert(1 <= x && x <= n && 1 <= y && y <= m);
        for (int i = x; i <= n; i += i & -i)
            for (int j = y; j <= m; j += j & -j) t[i][j] += v;
    }

    T ask(int x, int y) const {
        x = min(x, n), y = min(y, m);
        T r{};
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j) r += t[i][j];
        return r;
    }

    T ask(int x1, int y1, int x2, int y2) const {
        if (x1 > x2 || y1 > y2) return T{};
        assert(1 <= x1 && x2 <= n && 1 <= y1 && y2 <= m);
        return ask(x2, y2) - ask(x1 - 1, y2) - ask(x2, y1 - 1) + ask(x1 - 1, y1 - 1);
    }
};
