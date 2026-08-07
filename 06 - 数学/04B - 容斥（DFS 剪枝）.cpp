/*
用途：计算 [1,n] 中至少被一个 divisor 整除的数的个数。
复杂度：最坏 O(2^m)，但会在 lcm > n 时剪枝；适合 divisor 较多且 lcm 很快超界的情形。
与 04A 是同一问题的两种实现；m <= 22 且希望实现路径固定时使用 04A。
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
    i128 ans = 0;
    function<void(int, int, int)> dfs = [&](int st, int cur, int sgn) {
        for (int i = st; i < d.size(); ++i) {
            int nxt;
            if (!lcm(cur, d[i], n, nxt)) continue;
            ans += sgn * i128(n / nxt);
            dfs(i + 1, nxt, -sgn);
        }
    };
    dfs(0, 1, 1);
    assert(ans >= numeric_limits<int>::min() && ans <= numeric_limits<int>::max());
    return ans;
}
}  // namespace IE
