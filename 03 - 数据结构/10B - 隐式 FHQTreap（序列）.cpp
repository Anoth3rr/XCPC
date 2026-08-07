/*
用途：隐式 FHQ Treap，维护可修改序列。

下标均为 0-indexed，区间为半开 [l, r)。支持插入、删除、区间加、区间翻转、区间和与访问单点。
T 通常为 int；所有操作期望 O(log n)。删除节点不回收，适合竞赛一次性使用。
*/

template <class T = int>
struct FHQTreap {
    struct Node {
        int l = 0, r = 0;
        int sz = 0;
        int pri = 0;
        T v{}, sum{}, add{};
        bool rev = false;
    };

    vector<Node> tr = {Node{}};
    int rt = 0;
    mt19937 rng{chrono::steady_clock::now().time_since_epoch().count()};

    int sz(int p) const {
        return p ? tr[p].sz : 0;
    }
    T sum(int p) const {
        return p ? tr[p].sum : T{};
    }

    int node(const T &v) {
        tr.push_back(Node{});
        int p = tr.size() - 1;
        tr[p].v = tr[p].sum = v;
        tr[p].sz = 1;
        tr[p].pri = rng();
        return p;
    }

    void pull(int p) {
        if (!p) return;
        tr[p].sz = sz(tr[p].l) + 1 + sz(tr[p].r);
        tr[p].sum = sum(tr[p].l) + tr[p].v + sum(tr[p].r);
    }

    void setAdd(int p, const T &v) {
        if (!p) return;
        tr[p].v += v;
        tr[p].sum += v * tr[p].sz;
        tr[p].add += v;
    }

    void rev(int p) {
        if (!p) return;
        swap(tr[p].l, tr[p].r);
        tr[p].rev = !tr[p].rev;
    }

    void push(int p) {
        if (!p) return;
        if (tr[p].add != T{}) {
            setAdd(tr[p].l, tr[p].add);
            setAdd(tr[p].r, tr[p].add);
            tr[p].add = T{};
        }
        if (tr[p].rev) {
            rev(tr[p].l);
            rev(tr[p].r);
            tr[p].rev = false;
        }
    }

    // a 为前 k 个元素，b 为其余元素。
    void split(int p, int k, int &a, int &b) {
        if (!p) {
            a = b = 0;
            return;
        }
        push(p);
        if (sz(tr[p].l) >= k) {
            b = p;
            split(tr[p].l, k, a, tr[p].l);
            pull(b);
        } else {
            a = p;
            split(tr[p].r, k - sz(tr[p].l) - 1, tr[p].r, b);
            pull(a);
        }
    }

    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pri < tr[b].pri) {
            push(a);
            tr[a].r = merge(tr[a].r, b);
            pull(a);
            return a;
        }
        push(b);
        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }

    int askSz() const {
        return sz(rt);
    }

    void insert(int pos, const T &v) {
        assert(0 <= pos && pos <= askSz());
        int a, b;
        split(rt, pos, a, b);
        rt = merge(merge(a, node(v)), b);
    }

    void erase(int l, int r) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        rt = merge(a, c);
    }

    void update(int l, int r, const T &v) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        setAdd(b, v);
        rt = merge(merge(a, b), c);
    }

    void updRev(int l, int r) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        rev(b);
        rt = merge(merge(a, b), c);
    }

    T askSum(int l, int r) {
        assert(0 <= l && l <= r && r <= askSz());
        int a, b, c;
        split(rt, r, a, c);
        split(a, l, a, b);
        T ans = sum(b);
        rt = merge(merge(a, b), c);
        return ans;
    }

    optional<T> ask(int pos) {
        if (pos < 0 || pos >= askSz()) return nullopt;
        int a, b, c;
        split(rt, pos + 1, a, c);
        split(a, pos, a, b);
        T ans = tr[b].v;
        rt = merge(merge(a, b), c);
        return ans;
    }

    vector<T> askAll() {
        vector<T> res;
        res.reserve(askSz());
        auto dfs = [&](auto &&go, int p) -> void {
            if (!p) return;
            push(p);
            go(go, tr[p].l);
            res.push_back(tr[p].v);
            go(go, tr[p].r);
        };
        dfs(dfs, rt);
        return res;
    }
};
