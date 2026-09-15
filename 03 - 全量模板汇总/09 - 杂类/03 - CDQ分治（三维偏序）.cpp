/*
用途：统计三维偏序。返回 ans[k]：恰有 k 个其他点三维坐标均不大于它的点数。
复杂度：O(n log n)，重复点自动合并；坐标类型由 T 适配。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T>
struct CDQ3 {
    struct P {
        T y;
        int z, c, v;
    };

    vector<P> a, t;
    vector<int> bit;

    void modify(int x, int v) {
        for (++x; x < bit.size(); x += x & -x) bit[x] += v;
    }

    int ask(int x) const {
        int r = 0;
        for (++x; x; x -= x & -x) r += bit[x];
        return r;
    }

    void cdq(int l, int r) {
        if (r - l <= 1) return;
        int m = (l + r) / 2;
        cdq(l, m);
        cdq(m, r);
        int i = l, j = m, k = l;
        while (i < m && j < r) {
            if (a[i].y <= a[j].y) {
                modify(a[i].z, a[i].c);
                t[k++] = a[i++];
            } else {
                a[j].v += ask(a[j].z);
                t[k++] = a[j++];
            }
        }
        int p = i;
        while (j < r) {
            a[j].v += ask(a[j].z);
            t[k++] = a[j++];
        }
        while (i < m) t[k++] = a[i++];
        for (int x = l; x < p; ++x) modify(a[x].z, -a[x].c);
        copy(t.begin() + l, t.begin() + r, a.begin() + l);
    }

    vector<int> solve(vector<array<T, 3>> p) {
        int n = p.size();
        vector<int> ans(n);
        if (!n) return ans;
        sort(p.begin(), p.end());
        vector<T> z;
        z.reserve(n);
        for (auto& x : p) z.push_back(x[2]);
        sort(z.begin(), z.end());
        z.erase(unique(z.begin(), z.end()), z.end());

        a.clear();
        for (int i = 0; i < n;) {
            int j = i + 1;
            while (j < n && p[j] == p[i]) ++j;
            int k = lower_bound(z.begin(), z.end(), p[i][2]) - z.begin();
            a.push_back({p[i][1], k, j - i, 0});
            i = j;
        }
        t.resize(a.size());
        bit.assign(z.size() + 1, 0);
        cdq(0, a.size());
        for (auto& x : a) ans[x.v + x.c - 1] += x.c;
        return ans;
    }
};
