/*
用途：可移植的左偏可并堆。默认 Cmp = less<T> 时为大根堆；使用 greater<T> 可得到小根堆。

初始化 Heap(n) 后，h[i] 代表第 i 个堆，支持 push/pop/ask/merge/clear。
merge(x, y) 是破坏性合并，合并后 y 变为空堆。所有操作 O(log n)。
与 09B 解决同一问题；非 GCC/libstdc++ 环境使用本模板。
*/
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
