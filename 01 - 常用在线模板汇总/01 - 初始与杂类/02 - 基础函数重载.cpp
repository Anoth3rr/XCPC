int mypow(int a, int b, int m) {
    int r = 1 % m;
    for (; b; b >>= 1, a = a * a % m)
        if (b & 1) r = r * a % m;
    return r;
}
int mylcm(int x, int y) {
    return x / gcd(x, y) * y;
}
template <class T> int log2f(T n) {
    int ans = 0;
    while (n > 1) n >>= 1, ++ans;
    return ans;
}
int log2f(int n) {
    return 63 - __builtin_clzll(n);
}
template <class T> int log2c(T n) {
    return log2f(n) + ((n & (n - 1)) != 0);
}
int log2c(int n) {
    return log2f(n) + ((n & (n - 1)) != 0);
}
template <class T> T sign(const T &a) {
    return a == 0 ? 0 : (a < 0 ? -1 : 1);
}
template <class T> T floor(const T &a, const T &b) {
    T q = a / b, r = a % b;
    if (r != 0 && ((r > 0) != (b > 0))) --q;
    return q;
}
template <class T> T ceil(const T &a, const T &b) {
    T q = a / b, r = a % b;
    if (r != 0 && ((r > 0) == (b > 0))) ++q;
    return q;
}
int mysqrt(int n) {
    int ans = sqrt(n);
    while ((ans + 1) * (ans + 1) <= n) ans++;
    while (ans * ans > n) ans--;
    return ans;
}
template <typename T> bool In(T x, T a, T b) {
    return min(a, b) <= x && x <= max(a, b);
}
inline bool check(int x, int y, int n, int m) {
    return 0 <= x && x < n && 0 <= y && y < m;
}
template <class T, class... A> T encode(T bas, A... a) {
    T res = 0;
    ((res = res * bas + a), ...);
    return res;
}
inline char tomaj(char c) {
    if ('a' <= c && c <= 'z') return c - 32;
    return c;
}
inline char tolow(char c) {
    if ('A' <= c && c <= 'Z') return c + 32;
    return c;
}
template <typename... A> bool sameto(char c, A... a) {
    return ((a == c) && ...);
}
template <typename... A> bool exi(char c, A... a) {
    return ((a == c) || ...);
}