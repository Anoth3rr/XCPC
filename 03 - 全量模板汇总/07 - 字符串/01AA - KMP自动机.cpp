/*
用途：KMP 的 DFA 转移，适合数字 DP、避免/统计模式串出现次数。
用法：状态为已匹配前缀长度 0..|p|；askNxt 后用 ask 判断是否匹配完整模式串。
约定：字符集为连续 [F,F+A)，允许空模式串。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int A = 26, char F = 'a'> struct KMP {

    string p;
    vector<int> pi;
    vector<array<int, A>> to;

    KMP() = default;

    explicit KMP(const string& s) {
        build(s);
    }

    int id(char c) const {
        int x = c - F;
        assert(0 <= x && x < A);
        return x;
    }

    void build(const string& s) {
        p = s;
        int n = p.size();
        for (char c : p) id(c);
        pi.assign(n, 0);
        for (int i = 1, j = 0; i < n; ++i) {
            while (j && p[i] != p[j]) j = pi[j - 1];
            if (p[i] == p[j]) ++j;
            pi[i] = j;
        }
        to.assign(n + 1, array<int, A>());
        for (int i = 0; i <= n; ++i) {
            for (int x = 0; x < A; ++x) {
                int j = i;
                char c = static_cast<char>(F + x);
                while (j && (j == n || p[j] != c)) j = pi[j - 1];
                if (j < n && p[j] == c) ++j;
                to[i][x] = j;
            }
        }
    }

    int askSz() const {
        return p.size();
    }

    int askNxt(int p, int x) const {
        assert(0 <= p && p <= askSz() && 0 <= x && x < A);
        return to[p][x];
    }

    int askNxt(int p, char c) const {
        return askNxt(p, id(c));
    }

    bool ask(int p) const {
        return p == askSz();
    }
};
