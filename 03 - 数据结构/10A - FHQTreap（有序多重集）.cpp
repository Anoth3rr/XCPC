/*
用途：FHQ Treap 有序多重集，支持插入、删除、计数、排名、第 k 小、前驱和后继。

Key 需支持 <；askRank/askKth 为 1-indexed。erase(x,c) 删除至多 c 个。
所有操作期望 O(log n)，askKth/askPre/askNxt 不存在时返回 nullopt。
*/
template <class Key = int>
struct OSet {
    struct Node {
        int l = 0, r = 0;
        int sz = 0, cnt = 0;
        int pri = 0;
        Key key{};
    };

    vector<Node> tr = {Node{}};
    int rt = 0;
    mt19937 rng{chrono::steady_clock::now().time_since_epoch().count()};

    int sz(int p) const {
        return p ? tr[p].sz : 0;
    }

    void pull(int p) {
        if (p) tr[p].sz = tr[tr[p].l].sz + tr[p].cnt + tr[tr[p].r].sz;
    }

    int node(const Key &key, int cnt) {
        tr.push_back(Node{});
        int p = tr.size() - 1;
        tr[p].key = key;
        tr[p].cnt = tr[p].sz = cnt;
        tr[p].pri = rng();
        return p;
    }

    // a 中键 < key，b 中键 >= key。
    void split(int p, const Key &key, int &a, int &b) {
        if (!p) {
            a = b = 0;
        } else if (tr[p].key < key) {
            a = p;
            split(tr[p].r, key, tr[p].r, b);
            pull(a);
        } else {
            b = p;
            split(tr[p].l, key, a, tr[p].l);
            pull(b);
        }
    }

    // a 中键 <= key，b 中键 > key。
    void splitR(int p, const Key &key, int &a, int &b) {
        if (!p) {
            a = b = 0;
        } else if (!(key < tr[p].key)) {
            a = p;
            splitR(tr[p].r, key, tr[p].r, b);
            pull(a);
        } else {
            b = p;
            splitR(tr[p].l, key, a, tr[p].l);
            pull(b);
        }
    }

    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pri < tr[b].pri) {
            tr[a].r = merge(tr[a].r, b);
            pull(a);
            return a;
        }
        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }

    int askSz() const {
        return sz(rt);
    }

    bool askEmp() const {
        return rt == 0;
    }

    void clear() {
        tr.assign(1, Node{});
        rt = 0;
    }

    void insert(const Key &key, int cnt = 1) {
        if (cnt <= 0) return;
        int a, b, c;
        split(rt, key, a, b);
        splitR(b, key, b, c);
        if (b) tr[b].cnt += cnt, pull(b);
        else b = node(key, cnt);
        rt = merge(merge(a, b), c);
    }

    bool erase(const Key &key, int cnt = 1) {
        if (cnt <= 0) return false;
        int a, b, c;
        split(rt, key, a, b);
        splitR(b, key, b, c);
        bool ok = b;
        if (b) {
            if (tr[b].cnt > cnt) tr[b].cnt -= cnt, pull(b);
            else b = merge(tr[b].l, tr[b].r);
        }
        rt = merge(merge(a, b), c);
        return ok;
    }

    int askCnt(const Key &key) const {
        int p = rt;
        while (p) {
            if (key < tr[p].key) p = tr[p].l;
            else if (tr[p].key < key) p = tr[p].r;
            else return tr[p].cnt;
        }
        return 0;
    }

    int askRank(const Key &key) const {
        int p = rt, ans = 1;
        while (p) {
            if (!(tr[p].key < key)) {
                p = tr[p].l;
            } else {
                ans += sz(tr[p].l) + tr[p].cnt;
                p = tr[p].r;
            }
        }
        return ans;
    }

    optional<Key> askKth(int k) const {
        if (k <= 0 || k > askSz()) return nullopt;
        int p = rt;
        while (p) {
            int nl = sz(tr[p].l);
            if (k <= nl) p = tr[p].l;
            else if (k <= nl + tr[p].cnt) return tr[p].key;
            else k -= nl + tr[p].cnt, p = tr[p].r;
        }
        return nullopt;
    }

    optional<Key> askPre(const Key &key) const {
        int p = rt, ans = 0;
        bool ok = false;
        while (p) {
            if (tr[p].key < key) ans = p, ok = true, p = tr[p].r;
            else p = tr[p].l;
        }
        return ok ? optional<Key>(tr[ans].key) : nullopt;
    }

    optional<Key> askNxt(const Key &key) const {
        int p = rt, ans = 0;
        bool ok = false;
        while (p) {
            if (key < tr[p].key) ans = p, ok = true, p = tr[p].l;
            else p = tr[p].r;
        }
        return ok ? optional<Key>(tr[ans].key) : nullopt;
    }
};
