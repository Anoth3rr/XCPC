/*
用途：求 ax + by = gcd(a,b) 的一组系数，以及任意模数下的逆元。
inv(a,m) 有值当且仅当 gcd(a,m)=1；m 不要求为质数。
*/

struct ExgcdRes {
    int gcd, x, y;
};

inline ExgcdRes exgcd(int a, int b) {
    if (b == 0) return {a >= 0 ? a : -a, a >= 0 ? 1 : -1, 0};
    auto sub = exgcd(b, a % b);
    return {sub.gcd, sub.y, sub.x - (a / b) * sub.y};
}

inline optional<int> inv(int a, int mod) {
    assert(mod > 0);
    auto res = exgcd(a, mod);
    if (res.gcd != 1) return nullopt;
    int inv = res.x % mod;
    if (inv < 0) inv += mod;
    return inv;
}
