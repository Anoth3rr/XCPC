/*
用途：可持久化“计数 + 权值和”线段树（主席树）。

值域为半开区间 [L, R)，版本 0 为空树。
- modify(ver,p,v)：从旧版本克隆一个新版本，位置 p 的计数 +1、权值和 +v。
- ask(ver,l,r)：查询该版本在 [l,r) 的计数和权值和。
- askKth(x,y,k)：版本差 y-x 中第 k 小的位置（k 从 1 开始），非法返回 -1。
*/

struct SegTree {

    struct Info {
        int cnt = 0;
        int sum = 0;

        Info operator+(const Info &o) const {
            return {cnt + o.cnt, sum + o.sum};
        }
    };

    struct Node {
        int ch[2]{};
        Info val;
    };

    int lo = 0, hi = 0;
    vector<Node> tr;
    vector<int> rt;

    SegTree() = default;
    SegTree(int l, int r) {
        init(l, r);
    }

    void init(int l, int r) {
        assert(l < r);
        lo = l;
        hi = r;
        tr.assign(1, Node{});
        rt.assign(1, 0);
    }

    int clone(int p) {
        tr.push_back(tr[p]);
        return tr.size() - 1;
    }

    int modify0(int u, int l, int r, int p, int v) {
        int x = clone(u);
        ++tr[x].val.cnt;
        tr[x].val.sum += v;
        if (r - l == 1) return x;
        int mid = l + (r - l) / 2;
        int o = p >= mid;
        if (o) tr[x].ch[1] = modify0(tr[u].ch[1], mid, r, p, v);
        else tr[x].ch[0] = modify0(tr[u].ch[0], l, mid, p, v);
        return x;
    }

    Info ask0(int u, int l, int r, int ql, int qr) const {
        if (!u || qr <= l || r <= ql) return {};
        if (ql <= l && r <= qr) return tr[u].val;
        int mid = l + (r - l) / 2;
        return ask0(tr[u].ch[0], l, mid, ql, qr) + ask0(tr[u].ch[1], mid, r, ql, qr);
    }

    int askKth0(int x, int y, int l, int r, int k) const {
        int tot = tr[y].val.cnt - tr[x].val.cnt;
        if (k <= 0 || k > tot) return -1;
        while (r - l > 1) {
            int cnt = tr[tr[y].ch[0]].val.cnt - tr[tr[x].ch[0]].val.cnt;
            int mid = l + (r - l) / 2;
            if (k <= cnt) {
                x = tr[x].ch[0];
                y = tr[y].ch[0];
                r = mid;
            } else {
                k -= cnt;
                x = tr[x].ch[1];
                y = tr[y].ch[1];
                l = mid;
            }
        }
        return l;
    }

    int modify(int ver, int p, int v) {
        assert(0 <= ver && ver < rt.size());
        assert(lo <= p && p < hi);
        rt.push_back(modify0(rt[ver], lo, hi, p, v));
        return rt.size() - 1;
    }

    Info ask(int ver, int l, int r) const {
        assert(0 <= ver && ver < rt.size());
        l = max(l, lo);
        r = min(r, hi);
        return l >= r ? Info{} : ask0(rt[ver], lo, hi, l, r);
    }

    int askKth(int x, int y, int k) const {
        assert(0 <= x && x < rt.size());
        assert(0 <= y && y < rt.size());
        return askKth0(rt[x], rt[y], lo, hi, k);
    }

    int askCnt() const {
        return rt.size();
    }
};
