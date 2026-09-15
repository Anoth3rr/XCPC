template <class T> struct BIT {
    int n = 0, m = 0;
    array<vector<vector<T>>, 4> t;

    BIT() = default;
    BIT(int n, int m) {
        init(n, m);
    }

    void init(int _n, int _m) {
        assert(_n >= 0 && _m >= 0);
        n = _n, m = _m;
        for (auto &a : t) a.assign(n + 1, vector<T>(m + 1));
    }

    void add0(int k, int x, int y, const T &v) {
        for (int i = x; i <= n; i += i & -i)
            for (int j = y; j <= m; j += j & -j) a(k, i, j) += v;
    }

    T ask0(int k, int x, int y) const {
        T r{};
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j) r += t[k][i][j];
        return r;
    }

    T &a(int k, int x, int y) {
        return t[k][x][y];
    }

    void upd0(int x, int y, const T &v) {
        add0(0, x, y, v);
        add0(1, x, y, v * (x - 1));
        add0(2, x, y, v * (y - 1));
        add0(3, x, y, v * (x - 1) * (y - 1));
    }

    void update(int x1, int y1, int x2, int y2, const T &v) {
        if (x1 > x2 || y1 > y2) return;
        assert(1 <= x1 && x2 <= n && 1 <= y1 && y2 <= m);
        upd0(x1, y1, v);
        if (x2 < n) upd0(x2 + 1, y1, -v);
        if (y2 < m) upd0(x1, y2 + 1, -v);
        if (x2 < n && y2 < m) upd0(x2 + 1, y2 + 1, v);
    }

    T ask(int x, int y) const {
        x = min(x, n), y = min(y, m);
        if (x <= 0 || y <= 0) return T{};
        T p = ask0(0, x, y);
        return p * x * y - ask0(1, x, y) * y - ask0(2, x, y) * x + ask0(3, x, y);
    }

    T ask(int x1, int y1, int x2, int y2) const {
        if (x1 > x2 || y1 > y2) return T{};
        assert(1 <= x1 && x2 <= n && 1 <= y1 && y2 <= m);
        return ask(x2, y2) - ask(x1 - 1, y2) - ask(x2, y1 - 1) + ask(x1 - 1, y1 - 1);
    }
};
