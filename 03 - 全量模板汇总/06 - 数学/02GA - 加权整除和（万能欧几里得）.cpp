/*
用途：同时求 y_i=floor((a*i+b)/c)（0<=i<=n）的 sum y_i、sum i*y_i、sum y_i^2。
依赖：02A - 自动取模（MInt）。接口：Euclid<Z>::ask(a,b,c,n)。
前提：a,b,n 非负、c 正，且 a*n+b 可放入 int；Z 所在域中 2、6 可逆（通常用质数模 MInt）。
复杂度：O(log(max(a,c)))。这是 floor_sum 的带权扩展，可用于二次矩、矩阵转移前的系数统计。
*/
template <class Z> struct Euclid {
    struct Ans {
        Z s, t, u;
    };

    static Z sum1(int n) {
        return Z(n) * Z(n + 1) / Z(2);
    }

    static Z sum2(int n) {
        return Z(n) * Z(n + 1) * Z(n * 2 + 1) / Z(6);
    }

    static Ans ask(int a, int b, int c, int n) {
        assert(a >= 0 && b >= 0 && c > 0 && n >= 0);
        assert(Z(2) != Z(0) && Z(6) != Z(0));
        if (a >= c || b >= c) {
            int q = a / c, p = b / c;
            Ans x = ask(a % c, b % c, c, n);
            Z m = n + 1, s = sum1(n), t = sum2(n);
            Z qz = q, pz = p;
            return {
                qz * s + pz * m + x.s,
                qz * t + pz * s + x.t,
                qz * qz * t + pz * pz * m + Z(2) * qz * pz * s + Z(2) * qz * x.t + Z(2) * pz * x.s + x.u,
            };
        }
        if (!a) return {};
        int m = (a * n + b) / c;
        if (!m) return {};
        Ans x = ask(c, c - b - 1, a, m - 1);
        Z mz = m, nz = n;
        return {
            nz * mz - x.s,
            mz * sum1(n) - (x.u + x.s) / Z(2),
            nz * mz * mz - Z(2) * x.t - x.s,
        };
    }
};
