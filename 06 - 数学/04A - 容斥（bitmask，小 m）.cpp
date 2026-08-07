/*
用途：计算 [1,n] 中至少被一个 divisor 整除的数的个数。
复杂度：O(2^m * m)，m 为去重、去冗余后的 divisor 数，要求 m <= 22。
与 04B 是同一问题的两种实现；m 较大且 lcm 常很快超过 n 时可改用 04B。
*/

using i128 = __int128_t;

namespace IE {
inline vector<int> norm(vector<int> d) {
    for (int val : d) assert(val > 0);
    sort(d.begin(), d.end());
    d.erase(unique(d.begin(), d.end()), d.end());
    vector<int> res;
    for (int val : d) {
        bool bad = false;
        for (int kp : res) {
            if (val % kp == 0) {
                bad = true;
                break;
            }
        }
        if (!bad) res.push_back(val);
    }
    return res;
}

inline bool lcm(int a, int b, int lim, int& res) {
    int x = a / gcd(a, b);
    if (x > lim / b) return false;
    res = x * b;
    return true;
}

inline int solve(int n, vector<int> d) {
    assert(n >= 0);
    d = norm(move(d));
    int m = d.size();
    assert(m <= 22);
    i128 ans = 0;
    for (int msk = 1; msk < (1ULL << m); ++msk) {
        int cur = 1;
        bool ok = true;
        for (int i = 0; i < m; ++i) {
            if ((msk >> i & 1) && !lcm(cur, d[i], n, cur)) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;
        if (__builtin_popcountll(msk) & 1) ans += n / cur;
        else ans -= n / cur;
    }
    assert(ans >= numeric_limits<int>::min() && ans <= numeric_limits<int>::max());
    return ans;
}
}  // namespace IE
