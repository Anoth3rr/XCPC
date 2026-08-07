/*
用途：小且连续字符集上的子序列判定；以 O(nA) 空间换 O(m) 查询。
默认处理 'a'..'z'。状态和转移显式使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int A = 26, char F = 'a'> struct SubSeq {
    int n = 0;
    vector<array<int, A>> nx;

    SubSeq() = default;
    explicit SubSeq(const string& s) { build(s); }

    void build(const string& s) {
        n = s.size();
        nx.assign(n + 1, {});
        nx[n].fill(n);
        for (int i = n; i-- > 0;) {
            nx[i] = nx[i + 1];
            int c = static_cast<unsigned char>(s[i]) - static_cast<unsigned char>(F);
            assert(0 <= c && c < A);
            nx[i][c] = i;
        }
    }

    optional<int> askPos(const string& s) const {
        int p = 0, at = -1;
        for (unsigned char x : s) {
            int c = x - static_cast<unsigned char>(F);
            if (c < 0 || c >= A || p > n) return nullopt;
            int q = nx[p][c];
            if (q == n) return nullopt;
            at = q, p = q + 1;
        }
        return at;
    }

    bool ask(const string& s) const { return askPos(s).has_value(); }
};
