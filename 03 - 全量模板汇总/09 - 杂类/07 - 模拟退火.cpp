/*
用途：连续或离散状态的启发式优化；不保证正确，只用于允许随机化搜索的题。
go(s,t,rng) 返回邻居，f(s) 返回可转为 double 的目标值；MAX=true 时改为最大化。
随机数生成器可直接传入基础算法目录中的 Random::rng。
*/
template <bool MAX = false, class S, class F, class G, class R>
S anneal(S s, F f, G go, R& rng, double t = 1e4, double e = 1e-8,
         double a = 0.995, int rep = 1) {
    assert(t > e && 0 < a && a < 1 && rep > 0);
    uniform_real_distribution<double> u(0, 1);
    S st = s, ans = s;
    double bv = static_cast<double>(f(s));
    for (int z = 0; z < rep; ++z) {
        s = st;
        double v = static_cast<double>(f(s));
        for (double c = t; c > e; c *= a) {
            S ns = go(s, c, rng);
            double nv = static_cast<double>(f(ns));
            double d = nv - v;
            if constexpr (MAX) d = -d;
            if (d <= 0 || exp(-d / c) > u(rng)) s = std::move(ns), v = nv;
            bool ok = MAX ? v > bv : v < bv;
            if (ok) ans = s, bv = v;
        }
    }
    return ans;
}
