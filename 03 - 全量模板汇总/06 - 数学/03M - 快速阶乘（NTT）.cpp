/*
用途：在 NTT 友好质数模下求 n!。依赖：02A - 自动取模（MInt）、08B - 多项式（Poly, Z）。
接口：FastFac<Z>::ask(n)。n>=mod 时直接返回 0；n<mod 时使用分块多点求值。
复杂度：O(sqrt(n) log^2 n) 时间、O(sqrt(n)) 空间；小 n 直接循环通常更快。
*/
template <class Z, int G = 3> struct FastFac {
    static vector<Z> prod(int n) {
        vector<vector<Z>> a;
        a.reserve(n);
        for (int i = 1; i <= n; ++i) a.push_back({Z(i), Z(1)});
        while (a.size() > 1) {
            vector<vector<Z>> b;
            b.reserve((a.size() + 1) >> 1);
            for (int i = 0; i < a.size(); i += 2) {
                if (i + 1 == a.size()) b.push_back(move(a[i]));
                else b.push_back(Poly<Z, G>::mul(a[i], a[i + 1]));
            }
            a = move(b);
        }
        return a.empty() ? vector<Z>{Z(1)} : move(a[0]);
    }

    static Z ask(int n) {
        assert(n >= 0);
        if (!n) return Z(1);
        if (n >= Z::askMod()) return Z(0);
        int b = sqrtl(n) + 1;
        int q = n / b, r = n % b;
        vector<Z> f = prod(b), x(q);
        for (int i = 0; i < q; ++i) x[i] = Z(i * b);
        Z ans = 1;
        for (Z v : Poly<Z, G>::eval(f, x)) ans *= v;
        for (int i = 1; i <= r; ++i) ans *= Z(q * b + i);
        return ans;
    }
};
