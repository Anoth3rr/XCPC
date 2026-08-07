template <class T1, class T2> ostream &operator<<(ostream &os, const pair<T1, T2> &p) {
    return os << '{' << p.first << ", " << p.second << '}';
}

template <class T, class = decltype(begin(declval<T>())), class = enable_if_t<!is_same_v<T, string>>> ostream &operator<<(ostream &os, const T &c) {
    os << '[';
    for (auto it = c.begin(); it != c.end(); ++it) os << (it == c.begin() ? "" : ", ") << *it;
    return os << ']';
}

template <class... A> void _(A&&... a) {
    cerr << "---> ";
    ((cerr << forward<A>(a) << ' '), ...);
    cerr << "\n";
}

inline auto tic() {
    return chrono::steady_clock::now();
}

template <class T> void toc(T t, const char* s = "") {
    double x = chrono::duration<double, milli>(chrono::steady_clock::now() - t).count();
    cerr << "[time] " << s << (s[0] ? ": " : "") << fixed << setprecision(3) << x << " ms\n";
}
