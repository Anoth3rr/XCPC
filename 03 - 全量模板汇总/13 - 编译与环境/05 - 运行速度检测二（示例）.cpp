// #pragma GCC optimize("Ofast", "unroll-loops")

#include <bits/stdc++.h>
using namespace std;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

signed main() {
    size_t n = 340000000, seed = 0;
    for (int i = 1; i <= n; i++) {
        seed ^= rnd();
    }
    
    return 0;
}
