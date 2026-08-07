/*
用途：杜教筛求 M(n) = sum_{i<=n} mu(i)。
依赖：01 - 素数（线性筛 / Sieve）。预处理范围 lim 内 O(1)，超过后按整除分块递归并记忆化。
通常取 lim 约 n^(2/3) 或按内存设置；若只需 mu[1..N]，直接使用 Sieve::mu。
*/

struct Mertens {
    Sieve sv;
    vector<int> pre;
    unordered_map<int, int> mp;

    Mertens() = default;
    explicit Mertens(int lim) { init(lim); }

    void init(int lim) {
        assert(lim >= 1);
        sv.init(lim);
        pre.assign(lim + 1, 0);
        for (int i = 1; i <= lim; ++i) pre[i] = pre[i - 1] + sv.mu[i];
        mp.clear();
    }

    int ask(int n) {
        assert(n >= 0 && sv.n > 0);
        if (n <= sv.n) return pre[n];
        if (auto it = mp.find(n); it != mp.end()) return it->second;
        int ans = 1;
        for (int l = 2, r; l <= n; l = r + 1) {
            int q = n / l;
            r = n / q;
            ans -= (r - l + 1) * ask(q);
        }
        return mp[n] = ans;
    }
};
