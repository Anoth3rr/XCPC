struct Comb {
    int n;
    vector<Z> _fac, _invfac, _inv;

    Comb() : n{0}, _fac{1}, _invfac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }

    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _invfac.resize(m + 1);
        _inv.resize(m + 1);

        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _invfac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _invfac[i - 1] = _invfac[i] * i;
            _inv[i] = _invfac[i] * _fac[i - 1];
        }
        n = m;
    }

    Z fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    Z invfac(int m) {
        if (m > n) init(2 * m);
        return _invfac[m];
    }
    Z inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    Z P(int n, int m) {
        if (n < 0 || m < 0 || m > n) return 0;
        return fac(n) * invfac(n - m);
    }
    Z C(int n, int m) {
        if (n < 0 || m < 0 || m > n) return 0;
        return fac(n) * invfac(m) * invfac(n - m);
    }
} C;

auto C = [&](int n, int k) -> C {
    if (k < 0 || k > n) return Z(0);
    Z r = 1;
    while (n || k) {
        int x = n % p, y = k % p;
        if (y > x) return Z(0);
        r *= C.C(x, y);
        n /= p;
        k /= p;
    }
    return r;
}