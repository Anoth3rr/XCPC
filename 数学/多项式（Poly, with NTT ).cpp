template <ll MOD, ll G>
struct Poly {
    static void ntt(vector<ll> &a, bool invert) {
        int n = a.size();
        vector<int> rev(n);
        int lg = __builtin_ctz(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < lg; j++)
                if (i & (1 << j))
                    rev[i] |= 1 << (lg - 1 - j);
            if (i < rev[i])
                swap(a[i], a[rev[i]]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = ksm(G, (MOD - 1) / len, MOD);
            if (invert)
                wlen = ksm(wlen, MOD - 2, MOD);
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j];
                    ll v = a[i + j + len / 2] * w % MOD;
                    a[i + j] = (u + v) % MOD;
                    a[i + j + len / 2] = (u - v + MOD) % MOD;
                    w = w * wlen % MOD;
                }
            }
        }

        if (invert) {
            ll inv_n = ksm(n, MOD - 2);
            for (ll &x : a)
                x = x * inv_n % MOD;
        }
    }

    static vector<ll> add(vector<ll> a, vector<ll> b) {
        int n = max(a.size(), b.size());
        a.resize(n);
        b.resize(n);
        for (int i = 0; i < n; i++) {
            a[i] = (a[i] + b[i]) % MOD;
        }
        return a;
    }

    static vector<ll> sub(vector<ll> a, vector<ll> b) {
        int n = max(a.size(), b.size());
        a.resize(n);
        b.resize(n);
        for (int i = 0; i < n; i++) {
            a[i] = (a[i] - b[i] + MOD) % MOD;
        }
        return a;
    }

    static vector<ll> multiply(vector<ll> a, vector<ll> b) {
        int res_deg = (int)a.size() + (int)b.size() - 2;
        int sz = 1;
        while (sz < (int)a.size() + (int)b.size())
            sz <<= 1;
        a.resize(sz);
        b.resize(sz);
        ntt(a, false);
        ntt(b, false);
        for (int i = 0; i < sz; i++)
            a[i] = a[i] * b[i] % MOD;
        ntt(a, true);
        a.resize(res_deg + 1);
        return a;
    }

    static vector<ll> divide(const vector<ll> &a, const vector<ll> &b) {
        int n = a.size(), m = b.size();
        if (n < m)
            return {0};
        vector<ll> ra(a.rbegin(), a.rend());
        vector<ll> rb(b.rbegin(), b.rend());
        vector<ll> rb_inv;
        poly_inv(rb, rb_inv, n - m + 1);
        vector<ll> q = multiply(ra, rb_inv);
        q.resize(n - m + 1);
        reverse(q.begin(), q.end());
        return q;
    }

    static vector<ll> mod(const vector<ll> &a, const vector<ll> &b) {
        vector<ll> q = divide(a, b);
        vector<ll> r = minus(a, multiply(q, b));
        r.resize(min((int)r.size(), (int)b.size() - 1));
        return r;
    }

    static void poly_inv(const vector<ll> &a, vector<ll> &b, int deg) {
        if (deg == 1) {
            b.assign(1, ksm(a[0]));
            return;
        }
        poly_inv(a, b, (deg + 1) / 2);
        int sz = 1;
        while (sz < 2 * deg)
            sz <<= 1;
        vector<ll> a_slice(a.begin(), a.begin() + min((int)a.size(), deg));
        a_slice.resize(sz);
        b.resize(sz);
        ntt(a_slice, false);
        ntt(b, false);
        for (int i = 0; i < sz; i++) {
            b[i] = (2 * b[i] - a_slice[i] * b[i] % MOD * b[i] % MOD + MOD) % MOD;
        }
        ntt(b, true);
        b.resize(deg);
    }

    static vector<ll> derivative(const vector<ll> &a) {
        int n = a.size();
        if (n <= 1)
            return {};
        vector<ll> res(n - 1);
        for (int i = 1; i < n; i++)
            res[i - 1] = a[i] * i % MOD;
        return res;
    }

    static vector<ll> integral(const vector<ll> &a) {
        int n = a.size();
        vector<ll> res(n + 1);
        vector<ll> inv(n + 2);
        inv[1] = 1;
        for (int i = 2; i <= n + 1; i++)
            inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;
        for (int i = 0; i < n; i++)
            res[i + 1] = a[i] * inv[i + 1] % MOD;
        return res;
    }

    static void ln(const vector<ll> &a, vector<ll> &b, int deg) {
        vector<ll> a_der = derivative(a);
        vector<ll> a_inv;
        poly_inv(a, a_inv, deg);
        vector<ll> t = multiply(a_der, a_inv);
        t.resize(deg - 1);
        b = integral(t);
        b.resize(deg);
    }

    static void exp(const vector<ll> &a, vector<ll> &b, int deg) {
        if (deg == 1) {
            b.assign(1, 1);
            return;
        }
        exp(a, b, (deg + 1) / 2);
        b.resize(deg);
        vector<ll> ln_b;
        ln(b, ln_b, deg);
        vector<ll> a_slice(a.begin(), a.begin() + min((int)a.size(), deg));
        for (int i = 0; i < deg; i++) {
            ln_b[i] = (a_slice[i] - ln_b[i] + MOD) % MOD;
        }
        ln_b[0] = (ln_b[0] + 1) % MOD;
        b = multiply(b, ln_b);
        b.resize(deg);
    }

    static void build_eval_tree(vector<vector<ll>> &tree, const vector<ll> &xs, int u, int l, int r) {
        if (r - l == 1) {
            tree[u] = {(MOD - xs[l]) % MOD, 1};
        } else {
            int m = (l + r) / 2;
            build_eval_tree(tree, xs, 2 * u, l, m);
            build_eval_tree(tree, xs, 2 * u + 1, m, r);
            tree[u] = multiply(tree[2 * u], tree[2 * u + 1]);
        }
    }

    static void fast_eval_rec(const vector<ll> &f, const vector<vector<ll>> &tree, vector<ll> &res, int u, int l, int r) {
        if (r - l == 1) {
            res[l] = f.size() ? f[0] : 0;
            return;
        }
        vector<ll> rem = mod(f, tree[u]);
        int m = (l + r) / 2;
        fast_eval_rec(rem, tree, res, 2 * u, l, m);
        fast_eval_rec(rem, tree, res, 2 * u + 1, m, r);
    }

    static vector<ll> fast_eval(const vector<ll> &f, const vector<ll> &xs) {
        int n = xs.size();
        vector<vector<ll>> tree(4 * n);
        build_eval_tree(tree, xs, 1, 0, n);
        vector<ll> res(n);
        fast_eval_rec(f, tree, res, 1, 0, n);
        return res;
    }

    static vector<ll> interpolate(const vector<ll> &xs, const vector<ll> &ys) {
        int n = xs.size();
        vector<vector<ll>> tree(4 * n);
        build_eval_tree(tree, xs, 1, 0, n);
        vector<ll> all_poly = tree[1];
        vector<ll> der = derivative(all_poly);
        vector<ll> val = fast_eval(der, xs);
        vector<ll> weights(n);
        for (int i = 0; i < n; i++) {
            weights[i] = ys[i] * ksm(val[i]) % MOD;
        }

        function<vector<ll>(int, int, int)> solve = [&](int u, int l, int r) -> vector<ll> {
            if (r - l == 1) {
                return {weights[l]};
            }
            int m = (l + r) / 2;
            vector<ll> left = solve(2 * u, l, m);
            vector<ll> right = solve(2 * u + 1, m, r);
            return plus(multiply(left, tree[2 * u + 1]), multiply(right, tree[2 * u]));
        };

        return solve(1, 0, n);
    }
};

using NTT = Poly<MOD, G>;