/*
用途：求 sum_{0 <= i < n} floor((a*i+b)/m)，常用于整除分块、格点计数与欧几里得递归。
前提：n,m,a,b 均非负且 m > 0。返回 i128，调用方按答案范围输出。
复杂度：O(log(max(a,m)))。
*/

using i128 = __int128_t;

namespace FloorSum {
inline i128 flrSum(int n, int m, int a, int b) {
    assert(n >= 0 && m > 0 && a >= 0 && b >= 0);
    i128 cnt = n, mod = m, coe = a, off = b, ans = 0;
    while (true) {
        if (coe >= mod) {
            ans += (cnt - 1) * cnt * (coe / mod) / 2;
            coe %= mod;
        }
        if (off >= mod) {
            ans += cnt * (off / mod);
            off %= mod;
        }
        i128 mx = coe * cnt + off;
        if (mx < mod) break;
        cnt = mx / mod;
        off = mx % mod;
        swap(mod, coe);
    }
    return ans;
}
}  // namespace FloorSum
