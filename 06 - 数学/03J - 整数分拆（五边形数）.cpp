/*
用途：Euler 五边形数定理预处理整数分拆 p(0..n)。
Z 仅需支持加减；复杂度 O(n sqrt n)。
*/

template <class Z> vector<Z> part(int n) {
    assert(n >= 0);
    vector<Z> a(n + 1);
    a[0] = 1;
    for (int i = 1; i <= n; ++i) {
        Z s = 0;
        for (int k = 1;; ++k) {
            int x = k * (3 * k - 1) / 2;
            if (x > i) break;
            Z t = k & 1 ? Z(1) : Z(-1);
            s += t * a[i - x];
            int y = k * (3 * k + 1) / 2;
            if (y <= i) s += t * a[i - y];
        }
        a[i] = s;
    }
    return a;
}
