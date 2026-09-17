template <class T, class Cmp = less<T>> struct Heap {
    struct Node {
        T v;
        int l, r, d;
    };

    vector<Node> tr;
    vector<int> rt, siz;
    Cmp cmp;
    int tot;

    Heap() {}
    Heap(int n) {
        init(n);
    }

    void init(int n) {
        tr.assign(1, {});
        tr.reserve(n + 1);
        rt.assign(n + 1, 0);
        siz.assign(n + 1, 0);
        tot = 0;
    }

    int merge0(int x, int y) {
        if (!x || !y) return x | y;
        if (cmp(tr[x].v, tr[y].v)) swap(x, y);
        tr[x].r = merge0(tr[x].r, y);
        if (tr[tr[x].l].d < tr[tr[x].r].d) swap(tr[x].l, tr[x].r);
        tr[x].d = tr[tr[x].r].d + 1;
        return x;
    }

    void merge(int x, int y) {
        if (x == y) return;
        rt[x] = merge0(rt[x], rt[y]);
        siz[x] += siz[y];
        rt[y] = siz[y] = 0;
    }

    void push(int x, const T &v) {
        tr.push_back({v, 0, 0, 1});
        rt[x] = merge0(rt[x], ++tot);
        ++siz[x];
    }

    T ask(int x) {
        return tr[rt[x]].v;
    }

    T pop(int x) {
        int u = rt[x];
        T res = tr[u].v;
        rt[x] = merge0(tr[u].l, tr[u].r);
        --siz[x];
        return res;
    }

    void clear(int x) {
        rt[x] = siz[x] = 0;
    }

    bool askEmp(int x) {
        return siz[x] == 0;
    }

    int askSz(int x) {
        return siz[x];
    }

    struct Ref {
        Heap *h;
        int x;

        Ref &operator+=(Ref y) {
            h->merge(x, y.x);
            return *this;
        }

        Ref &operator<<(const T &v) {
            h->push(x, v);
            return *this;
        }

        T ask() {
            return h->ask(x);
        }

        T pop() {
            return h->pop(x);
        }

        void clear() {
            h->clear(x);
        }

        bool askEmp() {
            return h->askEmp(x);
        }

        int askSz() {
            return h->askSz(x);
        }
    };

    Ref operator[](int x) {
        return {this, x};
    }
};
