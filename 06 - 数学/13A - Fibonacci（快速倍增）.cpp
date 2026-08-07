/*
用途：O(log n) 求 Fibonacci；T 可为整数、MInt 或高精整数。
*/

template <class T> pair<T, T> fib2(int n) {
    assert(n >= 0);
    if (!n) return {T(0), T(1)};
    auto [x, y] = fib2<T>(n >> 1);
    T a = x * (T(2) * y - x);
    T b = x * x + y * y;
    return n & 1 ? pair<T, T>{b, a + b} : pair<T, T>{a, b};
}

template <class T> T fib(int n) {
    return fib2<T>(n).first;
}
