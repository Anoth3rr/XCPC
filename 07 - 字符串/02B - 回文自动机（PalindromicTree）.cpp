/*
用途：在线维护所有本质不同回文串、每个回文串的出现次数。
用法：append 后 askUni() 是本质不同回文数；askCnt()[state] 是该状态回文的总出现次数。
约定：字符集为连续 [F,F+A)，节点和出现次数使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int A = 26, char F = 'a'> struct PAM {

    struct Node {
        int len = 0, lk = 0, cnt = 0;
        array<int, A> to{};
    };

    vector<Node> t;
    vector<int> s;
    int lst = 1, mx = 0;

    PAM() {
        init();
    }

    void init() {
        t.assign(2, Node());
        t[0].len = -1;
        s.assign(1, -1);
        lst = 1;
        mx = 0;
    }

    int id(char c) const {
        int x = c - F;
        assert(0 <= x && x < A);
        return x;
    }

    int get(int p, int x) const {
        int n = s.size() - 1;
        while (s[n - 1 - t[p].len] != x) p = t[p].lk;
        return p;
    }

    int append(int x) {
        assert(0 <= x && x < A);
        s.push_back(x);
        int p = get(lst, x);
        if (t[p].to[x]) {
            lst = t[p].to[x];
            ++t[lst].cnt;
            mx = max(mx, t[lst].len);
            return lst;
        }
        int q = t.size();
        t.emplace_back();
        t[q].len = t[p].len + 2;
        t[p].to[x] = q;
        if (t[q].len == 1) {
            t[q].lk = 1;
        } else {
            p = get(t[p].lk, x);
            t[q].lk = t[p].to[x];
        }
        lst = q;
        ++t[lst].cnt;
        mx = max(mx, t[lst].len);
        return lst;
    }

    int append(char c) {
        return append(id(c));
    }

    void append(const string& a) {
        for (char c : a) append(c);
    }

    int askSz() const {
        return t.size();
    }

    int askUni() const {
        return askSz() - 2;
    }

    int askMax() const {
        return mx;
    }

    vector<int> askOrd() const {
        vector<int> c(s.size()), a(askSz() - 1);
        for (int i = 1; i < askSz(); ++i) ++c[t[i].len];
        for (int i = 1; i < c.size(); ++i) c[i] += c[i - 1];
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
};
