/*
用途：已知 n = Π p_i^{e_i} 的质因数分解后，枚举全部约数并计算约数个数、约数和、欧拉函数。
依赖：分解可由 01A（小范围）或 01FA（64 位）得到。约数和以 i128 返回，避免中间溢出。
复杂度：枚举 O(tau(n))，其余 O(质因子种数)。
*/

using i128 = __int128_t;

namespace Divisor {
using Factor = vector<pair<int, int>>;

inline void check(const Factor& fac) {
    for (auto [pri, e] : fac) assert(pri > 1 && e >= 1);
}

inline vector<int> getDiv(const Factor& fac) {
    check(fac);
    vector<int> d{1};
    for (auto [pri, e] : fac) {
        int n = d.size();
        int pw = 1;
        for (int i = 1; i <= e; ++i) {
            assert(pw <= numeric_limits<int>::max() / pri);
            pw *= pri;
            for (int i = 0; i < n; ++i) {
                assert(d[i] <= numeric_limits<int>::max() / pw);
                d.push_back(d[i] * pw);
            }
        }
    }
    sort(d.begin(), d.end());
    return d;
}

inline int askCnt(const Factor& fac) {
    check(fac);
    int ans = 1;
    for (auto [pri, e] : fac) {
        assert(ans <= numeric_limits<int>::max() / (e + 1));
        ans *= e + 1;
    }
    return ans;
}

inline i128 askSum(const Factor& fac) {
    check(fac);
    i128 ans = 1;
    for (auto [pri, e] : fac) {
        i128 pw = 1, sum = 1;
        for (int i = 1; i <= e; ++i) {
            pw *= pri;
            sum += pw;
        }
        ans *= sum;
    }
    return ans;
}

inline int phi(const Factor& fac) {
    check(fac);
    i128 ans = 1;
    for (auto [pri, e] : fac) {
        i128 pk = 1;
        for (int i = 1; i < e; ++i) pk *= pri;
        ans *= pk * (pri - 1);
    }
    assert(ans <= numeric_limits<int>::max());
    return ans;
}
}  // namespace Divisor
