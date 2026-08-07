/*
用途：多模式串匹配自动机。
用法：add 返回模式串终点；build 后 askCnt(text)[end] 是该模式串出现次数。
约定：节点编号、转移表和 BFS 队列使用 int；字符集可通过 A、F 适配。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int A = 26, char F = 'a'> struct ACAM {

    struct Node {
        int len = 0, lk = 0;
        array<int, A> to{};
    };

    vector<Node> t;
    vector<int> ord;
    bool ok = false;

    ACAM() {
        init();
    }

    void init() {
        t.assign(2, Node());
        t[0].to.fill(1);
        t[0].len = -1;
        ord.clear();
        ok = false;
    }

    int node() {
        t.emplace_back();
        return t.size() - 1;
    }

    int id(char c) const {
        int x = c - F;
        assert(0 <= x && x < A);
        return x;
    }

    int add(const string& s) {
        assert(!ok);
        assert(!s.empty());
        int p = 1;
        for (char c : s) {
            int x = id(c);
            if (!t[p].to[x]) {
                t[p].to[x] = node();
                t[t[p].to[x]].len = t[p].len + 1;
            }
            p = t[p].to[x];
        }
        return p;
    }

    void build() {
        if (ok) return;
        queue<int> q;
        q.push(1);
        ord.clear();
        ord.reserve(t.size() - 1);
        ord.push_back(1);
        while (!q.empty()) {
            int x = q.front();
            q.pop();
            for (int i = 0; i < A; ++i) {
                if (!t[x].to[i]) {
                    t[x].to[i] = t[t[x].lk].to[i];
                } else {
                    t[t[x].to[i]].lk = t[t[x].lk].to[i];
                    q.push(t[x].to[i]);
                    ord.push_back(t[x].to[i]);
                }
            }
        }
        ok = true;
    }

    int askNxt(int p, int x) const {
        assert(0 <= p && p < askSz() && 0 <= x && x < A);
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

    const vector<int>& askOrd() const {
        assert(ok);
        return ord;
    }

    vector<int> askCnt(const string& s) const {
        assert(ok);
        vector<int> a(askSz());
        int p = 1;
        for (char c : s) {
            p = askNxt(p, c);
            ++a[p];
        }
        for (int i = ord.size(); --i > 0;) {
            int u = ord[i];
            a[t[u].lk] += a[u];
        }
        return a;
    }
};
