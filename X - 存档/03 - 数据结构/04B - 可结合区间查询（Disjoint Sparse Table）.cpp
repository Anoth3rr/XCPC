template <class T, class Op> struct DST {
    int n = 0, lg = 0;
    Op op;
    vector<T> val;
    vector<vector<T>> tab;

    DST() = default;
    DST(const vector<T> &a, Op op = Op()) : op(move(op)) {
        build(a);
    }

    void build(const vector<T> &a) {
        val = a;
        n = val.size();
        if (n == 0) {
            lg = 0;
            tab.clear();
            return;
        }
        lg = __lg(max<int>(1, n - 1)) + 1;
        tab.assign(lg, vector<T>(n));
        for (int lev = 0; lev < lg; ++lev) {
            int h = 1 << lev;
            int blk = h << 1;
            for (int beg = 0; beg < n; beg += blk) {
                int mid = min(beg + h, n);
                int end = min(beg + blk, n);
                if (mid > beg) {
                    tab[lev][mid - 1] = val[mid - 1];
                    for (int i = mid - 2; i >= beg; --i) tab[lev][i] = op(val[i], tab[lev][i + 1]);
                }
                if (mid < end) {
                    tab[lev][mid] = val[mid];
                    for (int i = mid + 1; i < end; ++i) tab[lev][i] = op(tab[lev][i - 1], val[i]);
                }
            }
        }
    }

    T ask(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        if (l == r) return val[l];
        int lev = __lg(l ^ r);
        return op(tab[lev][l], tab[lev][r]);
    }
};
