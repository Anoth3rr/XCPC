/*
用途：后缀自动机，append 后可求不同子串数及各状态出现次数。
约定：状态编号、转移和出现次数使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int A = 26, char F = 'a'> struct SAM {

    struct Node {
        int len = 0, lk = 0, cnt = 0;
        array<int, A> to{};
    };

    vector<Node> t;
    int lst = 1;

    SAM() {
        init();
    }

    void init() {
        t.assign(2, Node());
        t[0].to.fill(1);
        t[0].len = -1;
        lst = 1;
    }

    int node() {
        t.emplace_back();
        return t.size() - 1;
    }

    int extend(int p, int c) {
        assert(0 <= p && p < askSz() && 0 <= c && c < A);
        if (t[p].to[c]) {
            int q = t[p].to[c];
            if (t[q].len == t[p].len + 1) return q;
            int r = node();
            t[r].len = t[p].len + 1;
            t[r].lk = t[q].lk;
            t[r].to = t[q].to;
            t[q].lk = r;
            while (t[p].to[c] == q) {
                t[p].to[c] = r;
                p = t[p].lk;
            }
            return r;
        }
        int q = node();
        t[q].len = t[p].len + 1;
        while (!t[p].to[c]) {
            t[p].to[c] = q;
            p = t[p].lk;
        }
        t[q].lk = extend(p, c);
        return q;
    }

    int id(char c) const {
        int x = c - F;
        assert(0 <= x && x < A);
        return x;
    }

    int extend(int p, char c) {
        return extend(p, id(c));
    }

    int append(char c) {
        lst = extend(lst, c);
        ++t[lst].cnt;
        return lst;
    }

    void append(const string& s) {
        for (char c : s) append(c);
    }

    int askNxt(int p, int x) const {
        return t[p].to[x];
    }

    int askNxt(int p, char c) const {
        return askNxt(p, id(c));
    }

    int askLink(int p) const {
        return t[p].lk;
    }

    int askLen(int p) const {
        return t[p].len;
    }

    int askSz() const {
        return t.size();
    }

    bool ask(const string& s) const {
        int p = 1;
        for (char c : s) {
            p = askNxt(p, c);
            if (!p) return false;
        }
        return true;
    }

    int askLCS(const string& s) const {
        int p = 1, w = 0, r = 0;
        for (char c : s) {
            int x = id(c);
            while (p != 1 && !t[p].to[x]) {
                p = t[p].lk;
                w = t[p].len;
            }
            if (t[p].to[x]) {
                p = t[p].to[x];
                ++w;
            } else {
                p = 1;
                w = 0;
            }
            r = max(r, w);
        }
        return r;
    }

    vector<int> askOrd() const {
        vector<int> c(askSz()), a(askSz() - 1);
        for (int i = 1; i < askSz(); ++i) ++c[t[i].len];
        for (int i = 1; i < askSz(); ++i) c[i] += c[i - 1];
        for (int i = 1; i < askSz(); ++i) a[--c[t[i].len]] = i;
        return a;
    }

    vector<int> askCnt() const {
        vector<int> a(askSz());
        for (int i = 1; i < askSz(); ++i) a[i] = t[i].cnt;
        auto o = askOrd();
        for (int i = o.size(); --i > 0;) {
            int p = o[i];
            a[t[p].lk] += a[p];
        }
        return a;
    }

    int askUni() const {
        int r = 0;
        for (int i = 2; i < askSz(); ++i) r += t[i].len - t[t[i].lk].len;
        return r;
    }
};
