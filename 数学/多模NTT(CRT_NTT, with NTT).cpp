struct CRT_NTT {
    using ll = long long;
    static constexpr int mod1 = 998244353;
    static constexpr int mod2 = 1004535809;
    static constexpr int mod3 = 104857601;

    NTT ntt1, ntt2, ntt3;

    ll crt(ll a, ll b, ll c, ll mod) {
        static const ll m1 = mod1;
        static const ll m2 = mod2;
        static const ll m3 = mod3;
        static const ll m1_inv_m2 = 332748118;
        static const ll m12_inv_m3 = 373914552;

        ll x = a;
        ll t = (b - x) * m1_inv_m2 % m2;
        if (t < 0)
            t += m2;
        x += t * m1;

        ll t2 = (c - x % m3) * m12_inv_m3 % m3;
        if (t2 < 0)
            t2 += m3;
        x += t2 * m1 * m2;

        return x % mod;
    }

    vector<ll> multiply(const vector<int> &a, const vector<int> &b, ll mod) {
        auto r1 = ntt1.multiply(a, b);
        auto r2 = ntt2.multiply(a, b);
        auto r3 = ntt3.multiply(a, b);

        int n = r1.size();
        vector<ll> res(n);
        for (int i = 0; i < n; i++)
            res[i] = crt(r1[i], r2[i], r3[i], mod);
        return res;
    }
};
