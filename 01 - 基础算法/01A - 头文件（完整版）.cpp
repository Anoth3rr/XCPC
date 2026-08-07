#include <bits/stdc++.h>
using namespace std;

#ifndef ONLINE_JUDGE
    #include "linyue.hpp"
#else
    #define _(...)
#endif

namespace LYE {
#define int long long
#define pii array<int, 2>
#define endl "\n"
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define vi vector<int>
#define vvi vector<vector<int>>
#define pb push_back
#define pob pop_back
#define pf push_front
#define pof pop_front
    template <class T> istream &operator>>(istream &is, vector<T> &v) {
        for (auto &x : v) is >> x;
        return is;
    }
    template <typename T> T min(const vector<T> &v) {
        assert(!v.empty());
        return *std::min_element(v.begin(), v.end());
    }
    template <typename T> T max(const vector<T> &v) {
        assert(!v.empty());
        return *std::max_element(v.begin(), v.end());
    }
    template <typename T> T acc(const vector<T> &v) {
        return std::accumulate(v.begin(), v.end(), T{});
    }
    template <class T> void reverse(vector<T> &v) {
        std::reverse(v.begin(), v.end());
    }
    template <class T> void sort(vector<T> &v) {
        std::sort(v.begin(), v.end());
    }
    template <class T> void rsort(vector<T> &v) {
        std::sort(v.rbegin(), v.rend());
    }
    template <class T> void cmax(T &x, T y) {
        x = std::max(x, y);
    }
    template <class T> void cmin(T &x, T y) {
        x = std::min(x, y);
    }
    template <class T> T sign(const T &a) {
        return a == 0 ? 0 : (a < 0 ? -1 : 1);
    }
    template <class T> T floor(const T &a, const T &b) {
        assert(b != 0);
        assert(!(a == std::numeric_limits<T>::min() && b == -1));
        T q = a / b, r = a % b;
        if (r != 0 && ((r > 0) != (b > 0))) --q;
        return q;
    }
    template <class T> T ceil(const T &a, const T &b) {
        assert(b != 0);
        assert(!(a == std::numeric_limits<T>::min() && b == -1));
        T q = a / b, r = a % b;
        if (r != 0 && ((r > 0) == (b > 0))) ++q;
        return q;
    }
    int mypow(int n, int k, int p) {
        using i128 = __int128_t;
        assert(k >= 0 && p > 0);
        n %= p;
        if (n < 0) n += p;
        int r = 1 % p;
        while (k > 0) {
            if (k & 1) r = i128(r) * n % p;
            n = i128(n) * n % p;
            k >>= 1;
        }
        return r;
    }
    constexpr int N = 1e6 + 7;
    template <typename T = int> constexpr T INF = std::numeric_limits<T>::max() / 2;
}; // namespace LYE
using namespace LYE;

void solve() {}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout << fixed << setprecision(20);
    int t = 1;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        solve();
    }
    return 0;
}
