/*
莫比乌斯反演的两种常见方向（下标从 1 开始，mu 取 LinearSieve::mu）：
  F(n) = sum_{d|n} f(d)       <=> f(n) = sum_{d|n} mu(d) F(n/d)；
  G(n) = sum_{n|d} f(d)       <=> f(n) = sum_{k>=1} mu(k) G(nk)。
以下函数都是 O(N log N)，用于把公式直接落实为数组变换。
*/
template <class T> vector<T> zetaDiv(const vector<T>& f) {
    int n = f.size() - 1;
    assert(n >= 0);
    vector<T> res(n + 1, T(0));
    for (int d = 1; d <= n; ++d)
        for (int x = d; x <= n; x += d) res[x] += f[d];
    return res;
}

template <class T> vector<T> mobDiv(const vector<T>& F, const vector<int>& mu) {
    int n = F.size() - 1;
    assert(mu.size() > n);
    vector<T> res(n + 1, T(0));
    for (int d = 1; d <= n; ++d) {
        if (mu[d] == 0) continue;
        for (int x = d; x <= n; x += d) res[x] += T(mu[d]) * F[x / d];
    }
    return res;
}

template <class T> vector<T> zetaMul(const vector<T>& f) {
    int n = f.size() - 1;
    assert(n >= 0);
    vector<T> res(n + 1, T(0));
    for (int d = 1; d <= n; ++d)
        for (int x = d; x <= n; x += d) res[d] += f[x];
    return res;
}

template <class T> vector<T> mobMul(const vector<T>& G, const vector<int>& mu) {
    int n = G.size() - 1;
    assert(mu.size() > n);
    vector<T> res(n + 1, T(0));
    for (int k = 1; k <= n; ++k) {
        if (mu[k] == 0) continue;
        for (int d = 1; d * k <= n; ++d) res[d] += T(mu[k]) * G[d * k];
    }
    return res;
}
