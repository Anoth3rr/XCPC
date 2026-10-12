struct Montgomery {} M(10); // 注意预赋值
bool isprime(i64 n) {}
 
i64 rho(i64 n) {
    if (!(n & 1)) return 2;
    i64 x = 0, y = 0, prod = 1;
    auto f = [&](i64 x) -> i64 {
        return M.mul(x, x) + 5; // 这里的种子能被 hack ，如果是在线比赛，请务必 rand 生成
    };
    for (int t = 30, z = 0; t % 64 || gcd(prod, n) == 1; ++t) {
        if (x == y) x = ++z, y = f(x);
        if (i64 q = M.mul(prod, x + n - y)) prod = q;
        x = f(x), y = f(f(y));
    }
    return gcd(prod, n);
}
 
vector<i64> factorize(i64 x) {
    vector<i64> res;
    auto f = [&](auto f, i64 x) {
        if (x == 1) return;
        M = Montgomery(x); // 重设模数
        if (isprime(x)) return res.push_back(x);
        i64 y = rho(x);
        f(f, y), f(f, x / y);
    };
    f(f, x), sort(res.begin(), res.end());
    return res;
}
