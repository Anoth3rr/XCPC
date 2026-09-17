/*
用途：单模式串匹配、border 与前缀函数。
约定：前缀函数和匹配位置使用 int；空模式串匹配所有 n+1 个位置。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct KMP {

    string p;
    vector<int> pi;

    explicit KMP(const string& s) : p(s) {
        build();
    }

    void build() {
        int n = p.size();
        pi.assign(n, 0);
        for (int i = 1, j = 0; i < n; ++i) {
            while (j && p[i] != p[j]) j = pi[j - 1];
            if (p[i] == p[j]) ++j;
            pi[i] = j;
        }
    }

    vector<int> ask(const string& s) const {
        vector<int> a;
        int n = s.size(), m = p.size();
        if (!m) {
            a.resize(n + 1);
            iota(a.begin(), a.end(), 0);
            return a;
        }
        for (int i = 0, j = 0; i < n; ++i) {
            while (j && s[i] != p[j]) j = pi[j - 1];
            if (s[i] == p[j]) ++j;
            if (j == m) {
                a.push_back(i - m + 1);
                j = pi[j - 1];
            }
        }
        return a;
    }
};
