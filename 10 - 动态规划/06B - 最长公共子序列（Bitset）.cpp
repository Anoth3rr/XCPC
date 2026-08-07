/*
用途：只求两个字节串的 LCS 长度；利用 64 位并行优化。
复杂度：O(|a||b|/64 + |b|)，空间 O(256|b|/64)。
大位集必须使用 u64；若被 #define int long long 替换会显著增大内存和降低速度。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

int lcs(const string& a, const string& b) {
    int n = b.size(), z = (n + 63) >> 6;
    vector<vector<u64>> p(256, vector<u64>(z));
    for (int i = 0; i < n; ++i) p[(unsigned char)b[i]][i >> 6] |= u64(1) << (i & 63);
    vector<u64> s(z), x(z), y(z), d(z);
    for (unsigned char c : a) {
        u64 q = 1;
        for (int i = 0; i < z; ++i) {
            x[i] = p[c][i] | s[i];
            u64 nq = s[i] >> 63;
            y[i] = (s[i] << 1) | q;
            q = nq;
        }
        u64 br = 0;
        for (int i = 0; i < z; ++i) {
            u64 t = y[i] + br;
            u64 ov = t < y[i];
            d[i] = x[i] - t;
            br = ov || x[i] < t;
            s[i] = x[i] & ~d[i];
        }
    }
    int r = 0;
    for (u64 x : s) r += __builtin_popcountll(x);
    return r;
}
