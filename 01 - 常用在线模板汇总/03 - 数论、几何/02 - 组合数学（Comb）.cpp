struct Comb {
    int n;
    vector<Z> _fac, _ifac, _inv;

    Comb() : n{0}, _fac{1}, _ifac{1}, _inv{0} {}
    Comb(int n) : Comb() {
        init(n);
    }

    void init(int m) {
        if (m <= n) return;
        _fac.resize(m + 1);
        _ifac.resize(m + 1);
        _inv.resize(m + 1);

        for (int i = n + 1; i <= m; i++) {
            _fac[i] = _fac[i - 1] * i;
        }
        _ifac[m] = _fac[m].inv();
        for (int i = m; i > n; i--) {
            _ifac[i - 1] = _ifac[i] * i;
            _inv[i] = _ifac[i] * _fac[i - 1];
        }
        n = m;
    }

    Z fac(int m) {
        if (m > n) init(2 * m);
        return _fac[m];
    }
    Z ifac(int m) {
        if (m > n) init(2 * m);
        return _ifac[m];
    }
    Z inv(int m) {
        if (m > n) init(2 * m);
        return _inv[m];
    }
    Z P(int n, int m) {
        if (n < 0 || m < 0 || m > n) return 0;
        return fac(n) * ifac(n - m);
    }
    Z C(int n, int m) {
        if (n < 0 || m < 0 || m > n) return 0;
        return fac(n) * ifac(m) * ifac(n - m);
    }
} C;