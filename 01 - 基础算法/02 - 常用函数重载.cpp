// 根号
template <typename Int>
Int sqrt(Int n) {
    Int l = 0, r = 1e16, ans = l;
    while (l <= r) {
        Int mid = (l + r) >> 1;
        if (mid * mid <= n) {
            l = mid + 1;
            ans = mid;
        } else {
            r = mid - 1;
        }
    }
    return ans;
}
// 最大公因数
template <typename Int>
Int gcd(Int a, Int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}
// 最小公倍数
template <typename Int>
Int lcm(Int a, Int b) {
    return a / gcd(a, b) * b;
}
// a <= x && x <= b
template <typename Int>
bool In(Int x, Int a, Int b) {
    return min(a, b) <= x && x <= max(a, b);
}
// 在矩阵内
auto check = [&](int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
};
// 进制hash
auto encode = [&](int t, auto... args) {
    ll res = 0;
    ((res = res * t + args), ...);
    return res;
};
// 转大写
auto tomaj = [&](char c) -> char {
    if ('a' <= c && c <= 'z')
        return c - 32;
    return c;
};
// 转小写
auto tolow = [&](char c) -> char {
    if ('A' <= c && c <= 'Z')
        return c + 32;
    return c;
};
// 若干字符等于c
template <typename... Args>
bool sameto(char c, Args... args) {
    return ((args == c) && ...);
}
// 若干字符存在等于c
template <typename... Args>
bool exi(char c, Args... args) {
    return ((args == c) || ...);
}