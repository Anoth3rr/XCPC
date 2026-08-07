/*
用途：前缀版本主席树，维护值域 [L, R] 上的出现次数和附加权值和。

add(x,v) 追加一个前缀版本；ask(l,r,ql,qr) 查询第 l..r 个插入元素
（l/r 均为 1-indexed 闭区间）在值域 [ql, qr] 的计数和权值和。
*/

struct SegTree {
    struct Info {
        int cnt = 0;
        int sum = 0;
        Info operator+(const Info &o) const {
            return {cnt + o.cnt, sum + o.sum};
        }
    };

    int lo, hi;
    vector<array<int, 2>> ch;
    vector<int> cnt, rt;
    vector<int> sum;

    SegTree() {}
    SegTree(int l, int r) {
        init(l, r);
    }

    void init(int l, int r) {
        lo = l;
        hi = r;
        ch.assign(1, {0, 0});
        cnt.assign(1, 0);
        sum.assign(1, 0);
        rt.assign(1, 0);
    }

    int clone(int p) {
        ch.push_back(ch[p]);
        cnt.push_back(cnt[p]);
        sum.push_back(sum[p]);
        return cnt.size() - 1;
    }

    int modify(int p, int l, int r, int x, int v) {
        int q = clone(p);
        ++cnt[q];
        sum[q] += v;
        if (l < r) {
            int mid = (l + r) >> 1;
            if (x <= mid) {
                ch[q][0] = modify(ch[p][0], l, mid, x, v);
            } else {
                ch[q][1] = modify(ch[p][1], mid + 1, r, x, v);
            }
        }
        return q;
    }

    int modify(int pre, int x, int v) {
        return modify(pre, lo, hi, x, v);
    }

    int add(int x) {
        rt.push_back(modify(rt.back(), x, x));
        return rt.back();
    }

    int add(int x, int v) {
        rt.push_back(modify(rt.back(), x, v));
        return rt.back();
    }

    Info ask0(int u, int v, int l, int r, int ql, int qr) const {
        if (ql > qr || qr < l || r < ql) {
            return {};
        }
        if (ql <= l && r <= qr) {
            return {cnt[u] - cnt[v], sum[u] - sum[v]};
        }
        int mid = (l + r) >> 1;
        return ask0(ch[u][0], ch[v][0], l, mid, ql, qr) +
               ask0(ch[u][1], ch[v][1], mid + 1, r, ql, qr);
    }

    Info ask0(int u, int v, int ql, int qr) const {
        ql = max(ql, lo);
        qr = min(qr, hi);
        if (ql > qr) {
            return {};
        }
        return ask0(u, v, lo, hi, ql, qr);
    }

    Info ask(int l, int r, int ql, int qr) const {
        if (l > r) {
            return {};
        }
        return ask0(rt[r], rt[l - 1], ql, qr);
    }

    int askCnt() const {
        return rt.size() - 1;
    }
};
