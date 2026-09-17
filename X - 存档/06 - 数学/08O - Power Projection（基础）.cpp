/*
用途：求 h(i)=[x^N] f(x)g(x)^i（0<=i<=n）。依赖：02A - 自动取模（MInt）、08B - 多项式（Poly, Z）。
接口：PowerProj<Z>::ask(f,g,N,n)。f/g 的 N 次以上系数自动忽略。
复杂度：O(n*N*log N)，适合 n 或 N 中至少一个较小的场景；两者都很大时需专项分治实现。
*/
template <class Z, int G = 3> struct PowerProj {
    static vector<Z> ask(vector<Z> f, vector<Z> g, int N, int n) {
        assert(N >= 0 && n >= 0);
        f.resize(N + 1);
        g.resize(N + 1);
        vector<Z> cur{Z(1)}, ans(n + 1);
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= N; ++j) {
                int k = N - j;
                if (k < cur.size()) ans[i] += f[j] * cur[k];
            }
            if (i == n) break;
            cur = Poly<Z, G>::mul(cur, g);
            cur.resize(N + 1);
        }
        return ans;
    }
};
