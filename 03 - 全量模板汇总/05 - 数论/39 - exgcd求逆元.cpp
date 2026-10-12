int exgcd(int a, int b, int &x, int &y) {
    if (!b) {
        x = 1, y = 0;
        return a;
    }
    int d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
int inv(int a, int m) {
    int x, y, d = exgcd(a, m, x, y);
    return d == 1 ? (x % m + m) % m : -1;
}
