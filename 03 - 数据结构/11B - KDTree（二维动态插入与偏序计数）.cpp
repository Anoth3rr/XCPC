/*
用途：二维动态 KD Tree，插入点并查询 x<=qx 且 y<=qy 的点数；重复点分别计数。
复杂度：替罪羊式失衡重构，均摊表现良好；单次查询最坏仍可能 O(n)。
注意：节点下标显式 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class T = int> struct KDTree {
    struct Node {
        T x = 0, y = 0, ax = 0, bx = 0, ay = 0, by = 0;
        int l = 0, r = 0, sz = 1, d = 0;
    };

    vector<Node> t{{}};
    int rt = 0;

    int sz(int p) const { return p ? t[p].sz : 0; }

    void pull(int p) {
        auto &u = t[p];
        u.sz = 1;
        u.ax = u.bx = u.x;
        u.ay = u.by = u.y;
        for (int v : {u.l, u.r}) if (v) {
            u.sz += t[v].sz;
            u.ax = min(u.ax, t[v].ax);
            u.bx = max(u.bx, t[v].bx);
            u.ay = min(u.ay, t[v].ay);
            u.by = max(u.by, t[v].by);
        }
    }

    int build(vector<int> &a, int l, int r, int d = 0) {
        if (l >= r) return 0;
        int m = (l + r) >> 1;
        nth_element(a.begin() + l, a.begin() + m, a.begin() + r, [&](int p, int q) {
            if (!d) return pair(t[p].x, t[p].y) < pair(t[q].x, t[q].y);
            return pair(t[p].y, t[p].x) < pair(t[q].y, t[q].x);
        });
        int p = a[m];
        t[p].d = d;
        t[p].l = build(a, l, m, d ^ 1);
        t[p].r = build(a, m + 1, r, d ^ 1);
        pull(p);
        return p;
    }

    void flat(int p, vector<int> &a) {
        if (!p) return;
        flat(t[p].l, a);
        a.push_back(p);
        flat(t[p].r, a);
    }

    void rebuild(int &p) {
        int d = t[p].d;
        vector<int> a;
        a.reserve(t[p].sz);
        flat(p, a);
        p = build(a, 0, a.size(), d);
    }

    bool bad(int p) const { return max(sz(t[p].l), sz(t[p].r)) * 4 > sz(p) * 3; }

    void ins(int &p, int q) {
        if (!p) {
            p = q;
            pull(p);
            return;
        }
        if ((!t[p].d && pair(t[q].x, t[q].y) < pair(t[p].x, t[p].y)) ||
            (t[p].d && pair(t[q].y, t[q].x) < pair(t[p].y, t[p].x)))
            ins(t[p].l, q);
        else
            ins(t[p].r, q);
        pull(p);
        if (bad(p)) rebuild(p);
    }

    void insert(T x, T y) {
        Node u;
        u.x = u.ax = u.bx = x;
        u.y = u.ay = u.by = y;
        t.push_back(u);
        ins(rt, t.size() - 1);
    }

    int ask0(int p, T x, T y) const {
        if (!p || x < t[p].ax || y < t[p].ay) return 0;
        if (t[p].bx <= x && t[p].by <= y) return t[p].sz;
        return (t[p].x <= x && t[p].y <= y) + ask0(t[p].l, x, y) + ask0(t[p].r, x, y);
    }

    int ask(T x, T y) const { return ask0(rt, x, y); }
};
