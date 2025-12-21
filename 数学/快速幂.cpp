ll mul(ll a, ll b, ll p) {
    __int128 res = (__int128)a * b;
    res %= p;
    return res;
}

ll ksm(ll a, ll b, ll p) {
    ll res = 1 % p;
    while (b) {
        if (b & 1) res = mul(res, a, p);
        a = mul(a, a, p);
        b >>= 1;
    }
    return res;
}

template<typename Int>
Int ksm(Int a, ll b, Int p) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % p;
        a = a * a % p;
        b <<= 1;
    }
    return res;
}