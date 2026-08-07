/*
用途：二维静态 KD Tree，支持最近点（可排除一个编号）与闭矩形计数。
点下标为 build 时的 0-indexed 顺序。D 默认 i128，防止坐标差平方溢出 int。
平均查询接近 O(sqrt(n))；最坏 O(n)，构造 O(n log n)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using i128 = __int128_t;

template <class T = int, class D = i128> struct KDTree {
    struct Point {
        T x{}, y{};
    };
    struct Node {
        Point p;
        T mnx{}, mxx{}, mny{}, mxy{};
        int l = -1, r = -1, id = -1, sz = 1;
    };

    vector<pair<Point, int>> a;
    vector<Node> tr;
    int rt = -1;

    KDTree() = default;
    explicit KDTree(const vector<Point>& p) { build(p); }

    static D sq(T x, T y) {
        D d = D(x) - D(y);
        return d * d;
    }
    static D dis(const Point& a, const Point& b) { return sq(a.x, b.x) + sq(a.y, b.y); }

    void pull(int u) {
        Node& x = tr[u];
        x.mnx = x.mxx = x.p.x, x.mny = x.mxy = x.p.y, x.sz = 1;
        for (int v : {x.l, x.r}) if (v != -1) {
            x.mnx = min(x.mnx, tr[v].mnx), x.mxx = max(x.mxx, tr[v].mxx);
            x.mny = min(x.mny, tr[v].mny), x.mxy = max(x.mxy, tr[v].mxy);
            x.sz += tr[v].sz;
        }
    }

    int make(int l, int r, int d) {
        if (l >= r) return -1;
        int m = (l + r) >> 1;
        auto cmp = [d](const auto& x, const auto& y) {
            T a = d ? x.first.y : x.first.x;
            T b = d ? y.first.y : y.first.x;
            return a != b ? a < b : x.second < y.second;
        };
        nth_element(a.begin() + l, a.begin() + m, a.begin() + r, cmp);
        int u = tr.size();
        tr.push_back({a[m].first, a[m].first.x, a[m].first.x, a[m].first.y, a[m].first.y,
                      -1, -1, a[m].second, 1});
        tr[u].l = make(l, m, d ^ 1);
        tr[u].r = make(m + 1, r, d ^ 1);
        pull(u);
        return u;
    }

    void build(const vector<Point>& p) {
        a.clear(), tr.clear();
        a.reserve(p.size()), tr.reserve(p.size());
        for (int i = 0; i < p.size(); ++i) a.push_back({p[i], i});
        rt = make(0, a.size(), 0);
    }

    D box(int u, const Point& p) const {
        D r{};
        if (p.x < tr[u].mnx) r += sq(p.x, tr[u].mnx);
        if (p.x > tr[u].mxx) r += sq(p.x, tr[u].mxx);
        if (p.y < tr[u].mny) r += sq(p.y, tr[u].mny);
        if (p.y > tr[u].mxy) r += sq(p.y, tr[u].mxy);
        return r;
    }

    void near(int u, const Point& p, int ban, optional<D>& z) const {
        if (u == -1) return;
        if (tr[u].id != ban) {
            D d = dis(p, tr[u].p);
            if (!z || d < *z) z = d;
        }
        int x = tr[u].l, y = tr[u].r;
        D dx = x == -1 ? D(-1) : box(x, p);
        D dy = y == -1 ? D(-1) : box(y, p);
        if (y != -1 && (x == -1 || dy < dx)) swap(x, y), swap(dx, dy);
        if (x != -1 && (!z || dx <= *z)) near(x, p, ban, z);
        if (y != -1 && (!z || dy <= *z)) near(y, p, ban, z);
    }

    optional<D> askNear(const Point& p, int ban = -1) const {
        optional<D> r;
        near(rt, p, ban, r);
        return r;
    }

    int rect(int u, T x1, T y1, T x2, T y2) const {
        if (u == -1 || tr[u].mxx < x1 || x2 < tr[u].mnx || tr[u].mxy < y1 || y2 < tr[u].mny) return 0;
        if (x1 <= tr[u].mnx && tr[u].mxx <= x2 && y1 <= tr[u].mny && tr[u].mxy <= y2) return tr[u].sz;
        int r = x1 <= tr[u].p.x && tr[u].p.x <= x2 && y1 <= tr[u].p.y && tr[u].p.y <= y2;
        return r + rect(tr[u].l, x1, y1, x2, y2) + rect(tr[u].r, x1, y1, x2, y2);
    }

    int ask(T x1, T y1, T x2, T y2) const {
        if (x1 > x2 || y1 > y2) return 0;
        return rect(rt, x1, y1, x2, y2);
    }
};
