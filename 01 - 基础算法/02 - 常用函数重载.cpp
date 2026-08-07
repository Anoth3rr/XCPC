using i128 = __int128_t;

// 常用整型辅助函数与无上下文的小工具。
// 根号

int mysqrt(int n) {
    assert(n >= 0);
    int ans = sqrtl(n);
    while (i128(ans + 1) * (ans + 1) <= n) ans++;
    while (i128(ans) * ans > n) ans--;
    return ans;
}
// 最小公倍数（约定返回非负值）。
int mylcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int ans = a / gcd(a, b) * b;
    return ans < 0 ? -ans : ans;
}
// log2向下取整
template <class T> int log2f(T n) {
    assert(n > 0);
    int ans = 0;
    while (n > 1) n >>= 1, ++ans;
    return ans;
}
int log2f(int n) {
    assert(n > 0);
    return 63 - __builtin_clzll(n);
}
// log2向上取整
template <class T> int log2c(T n) {
    assert(n > 0);
    return log2f(n) + ((n & (n - 1)) != 0);
}
int log2c(int n) {
    assert(n > 0);
    return log2f(n) + ((n & (n - 1)) != 0);
}
template <class T> T sign(const T &a) {
    return a == 0 ? 0 : (a < 0 ? -1 : 1);
}
template <class T> T floor(const T &a, const T &b) {
    assert(b != 0);
    assert(!(a == std::numeric_limits<T>::min() && b == -1));
    T q = a / b, r = a % b;
    if (r != 0 && ((r > 0) != (b > 0))) --q;
    return q;
}
template <class T> T ceil(const T &a, const T &b) {
    assert(b != 0);
    assert(!(a == std::numeric_limits<T>::min() && b == -1));
    T q = a / b, r = a % b;
    if (r != 0 && ((r > 0) == (b > 0))) ++q;
    return q;
}
// a <= x && x <= b
template <typename T> bool In(T x, T a, T b) {
    return std::min(a, b) <= x && x <= std::max(a, b);
}

// 矩阵边界（0-indexed）。
inline bool inGrid(int x, int y, int n, int m) {
    return 0 <= x && x < n && 0 <= y && y < m;
}

inline bool inSq(int x, int y, int n) {
    return inGrid(x, y, n, n);
}

// 把若干位按 base 进制编码；调用者保证不会溢出。
template <class T, class... A> T encode(T bas, A... a) {
    T res = 0;
    ((res = res * bas + static_cast<T>(a)), ...);
    return res;
}

// ASCII 转大写/小写。
inline char tomaj(char c) {
    if ('a' <= c && c <= 'z') return c - 32;
    return c;
}

inline char tolow(char c) {
    if ('A' <= c && c <= 'Z') return c + 32;
    return c;
}
// 若干字符等于c
template <typename... A> bool sameto(char c, A... a) {
    return ((a == c) && ...);
}
// 若干字符存在等于c
template <typename... A> bool exi(char c, A... a) {
    return ((a == c) || ...);
}
