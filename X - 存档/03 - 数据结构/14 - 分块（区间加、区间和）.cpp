template <class T = int> struct Block {
    int n, b, bc;
    vector<T> a, sm, tg;

    Block() : n(0), b(1), bc(0) {}

    explicit Block(const vector<T> &v) {
        init(v);
    }

    int id(int p) const {
        return (p - 1) / b;
    }

    int lb(int x) const {
        return x * b + 1;
    }

    int rb(int x) const {
        return min(n, (x + 1) * b);
    }

    void init(const vector<T> &v) {
        assert(v.size() >= 2);
        n = v.size() - 1;
        b = sqrtl(n) + 1;
        bc = (n + b - 1) / b;
        a = v;
        sm.assign(bc, T{});
        tg.assign(bc, T{});
        for (int i = 1; i <= n; ++i) sm[id(i)] += a[i];
    }

    void push(int x) {
        if (tg[x] == T{}) return;
        for (int i = lb(x); i <= rb(x); ++i) a[i] += tg[x];
        tg[x] = T{};
    }

    void pull(int x) {
        sm[x] = T{};
        for (int i = lb(x); i <= rb(x); ++i) sm[x] += a[i];
    }

    void update(int l, int r, const T &v) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        int x = id(l), y = id(r);
        if (x == y) {
            push(x);
            for (int i = l; i <= r; ++i) a[i] += v;
            pull(x);
            return;
        }
        push(x);
        for (int i = l; i <= rb(x); ++i) a[i] += v;
        pull(x);
        push(y);
        for (int i = lb(y); i <= r; ++i) a[i] += v;
        pull(y);
        for (int z = x + 1; z < y; ++z) {
            tg[z] += v;
            sm[z] += v * (rb(z) - lb(z) + 1);
        }
    }

    void modify(int p, const T &v) {
        assert(1 <= p && p <= n);
        int x = id(p);
        push(x);
        sm[x] += v - a[p];
        a[p] = v;
    }

    T ask(int l, int r) {
        if (l > r) return T{};
        assert(1 <= l && r <= n);
        int x = id(l), y = id(r);
        T ans{};
        if (x == y) {
            push(x);
            for (int i = l; i <= r; ++i) ans += a[i];
            return ans;
        }
        push(x);
        for (int i = l; i <= rb(x); ++i) ans += a[i];
        push(y);
        for (int i = lb(y); i <= r; ++i) ans += a[i];
        for (int z = x + 1; z < y; ++z) ans += sm[z];
        return ans;
    }
};
