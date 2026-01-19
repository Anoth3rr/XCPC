ll mul(ll a, ll b, ll p) {
    __int128 res = (__int128)a * b;
    res %= p;
    return res;
}

ll ksm(ll a, ll b, ll p) {
    ll res = 1 % p;
    while (b) {
        if (b & 1)
            res = mul(res, a, p);
        a = mul(a, a, p);
        b >>= 1;
    }
    return res;
}

ll inv(ll x, ll p) {
    return ksm(x, p - 2, p);
}