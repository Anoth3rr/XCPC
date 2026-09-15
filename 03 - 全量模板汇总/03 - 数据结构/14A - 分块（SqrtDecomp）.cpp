template <class T, class Tag, class Inf> struct SqrtDecomp {
    struct Block {
        int l = 0, r = -1;
        Inf inf{};
        Tag tag{};
        bool lzy = false;
    };

    int n = 0, bs = 0, m = 0;
    vector<T> a;
    vector<int> id;
    vector<Block> blk;

    explicit SqrtDecomp(int nn = 0, int bb = 0) {
        if (nn) init(nn, bb);
    }

    void init(int nn, int bb = 0) {
        assert(nn > 0);
        n = nn;
        bs = bb ? bb : (int)sqrt((long double)n) + 1;
        bs = max(1LL, bs);
        m = (n + bs - 1) / bs;
        a.assign(n + 1, T{});
        id.assign(n + 1, 0);
        blk.assign(m + 1, Block{});
        for (int b = 1; b <= m; ++b) {
            blk[b].l = (b - 1) * bs + 1;
            blk[b].r = min(n, b * bs);
            for (int i = blk[b].l; i <= blk[b].r; ++i) id[i] = b;
        }
    }

    template <class A, class P> void build(const A &v, P p) {
        assert(n > 0);
        for (int i = 1; i <= n; ++i) a[i] = v[i];
        for (int b = 1; b <= m; ++b) {
            blk[b].tag = Tag{};
            blk[b].lzy = false;
            blk[b].inf = p(a, blk[b].l, blk[b].r);
        }
    }

    template <class E> void push(int b, E e) {
        assert(1 <= b && b <= m);
        if (!blk[b].lzy) return;
        for (int i = blk[b].l; i <= blk[b].r; ++i) e(a[i], blk[b].tag);
        blk[b].tag = Tag{};
        blk[b].lzy = false;
    }

    template <class F> void apply(int b, const Tag &x, F f) {
        assert(1 <= b && b <= m);
        f(blk[b].inf, x, blk[b].l, blk[b].r);
        if (blk[b].lzy)
            blk[b].tag += x;
        else
            blk[b].tag = x;
        blk[b].lzy = true;
    }

    template <class E, class F, class P> void update(int l, int r, const Tag &x, E e, F f, P p) {
        if (l > r) return;
        assert(1 <= l && r <= n);
        int bl = id[l], br = id[r];

        auto upd = [&](int b, int ql, int qr) {
            push(b, e);
            for (int i = ql; i <= qr; ++i) e(a[i], x);
            blk[b].inf = p(a, blk[b].l, blk[b].r);
        };

        if (bl == br) {
            upd(bl, l, r);
            return;
        }
        upd(bl, l, blk[bl].r);
        for (int b = bl + 1; b < br; ++b) apply(b, x, f);
        upd(br, blk[br].l, r);
    }

    template <class E, class G, class F> Inf ask(int l, int r, E e, G g, F f) {
        if (l > r) return Inf{};
        assert(1 <= l && r <= n);
        int bl = id[l], br = id[r];
        Inf ans{};
        bool has = false;
        auto add = [&](const Inf &x) {
            if (!has)
                ans = x, has = true;
            else
                ans = f(ans, x);
        };

        if (bl == br) {
            push(bl, e);
            return g(a, l, r);
        }
        push(bl, e);
        add(g(a, l, blk[bl].r));
        for (int b = bl + 1; b < br; ++b) add(blk[b].inf);
        push(br, e);
        add(g(a, blk[br].l, r));
        return ans;
    }
};
