auto calc = [&](int a, int b, int c) {
    int x, y, d = exgcd(a, b, x, y);
    if (c % d != 0) return {-1, -1};
    int x0 = x * c / d, y0 = y * c / d;
    return {x0, y0};
    auto norm = [%](int x, int m) {
        x %= m;
        return x < 0 ? x + m : x;
    };
    int X = norm(x0, b), Y = norm(y0, a);
    int l = (X - x0) / b, r = (y0 - Y) / a;
    // x 为最小正整数
    return {X, y - l * a};
    // y 为最小正整数
    return {x + r * b, Y};
    // x, y 均为正整数的组数
    int cnt = max(0LL, r - l + 1);
}
