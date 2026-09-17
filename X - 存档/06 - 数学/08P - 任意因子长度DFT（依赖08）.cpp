/*
用途：长度 n 整除 P-1 的 DFT/IDFT，不局限于 2 的幂。
依赖：08 - 快速数论变换（NTT）。用法：DFT<998244353, 3>::dft(a, false/true)。
复杂度：可分解为小因子时 O(n log n)；大素因子会退化为 O(r^2) 的朴素 DFT。
限制：P 必须为 NTT 友好质数，输入系数先规范到 [0,P)。
*/

template <int P, int G> struct DFT {
    static int pow(int a, int n) {
        return NTT<P, G>::pow(a, n);
    }

    static void naive(vector<int>& a, bool iv) {
        int n = a.size();
        if (n <= 1) return;
        int w = pow(G, (P - 1) / n);
        if (iv) w = pow(w, P - 2);
        vector<int> b(n);
        for (int i = 0; i < n; ++i) {
            int q = pow(w, i), z = 1;
            for (int j = 0; j < n; ++j) {
                b[i] = (b[i] + a[j] * z) % P;
                z = z * q % P;
            }
        }
        if (iv) {
            int z = pow(n, P - 2);
            for (int& x : b) x = x * z % P;
        }
        a.swap(b);
    }

    static int fac(int n) {
        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0) return i;
        return n;
    }

    static void dft(vector<int>& a, bool iv = false) {
        int n = a.size();
        if (!n) return;
        assert((P - 1) % n == 0);
        if (n <= 32) return naive(a, iv);
        if (!(n & (n - 1))) return NTT<P, G>::ntt(a, iv);

        int r = fac(n);
        if (r == n) return naive(a, iv);
        int m = n / r;
        vector<int> b(n), t(max(r, m));
        for (int x = 0; x < m; ++x) {
            t.resize(r);
            for (int y = 0; y < r; ++y) t[y] = a[x + m * y];
            dft(t, iv);
            for (int s = 0; s < r; ++s) b[s * m + x] = t[s];
        }

        int w = pow(G, (P - 1) / n);
        if (iv) w = pow(w, P - 2);
        for (int s = 0; s < r; ++s) {
            int q = pow(w, s), z = 1;
            t.resize(m);
            for (int x = 0; x < m; ++x) {
                t[x] = b[s * m + x] * z % P;
                z = z * q % P;
            }
            dft(t, iv);
            for (int x = 0; x < m; ++x) a[s + r * x] = t[x];
        }
    }
};
