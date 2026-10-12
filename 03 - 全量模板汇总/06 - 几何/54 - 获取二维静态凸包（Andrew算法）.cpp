template<class T> vector<Point<T>> staticConvexHull(vector<Point<T>> A, int flag = 1) {
    sort(A.begin(), A.end(), [](auto a, auto b) { return tie(a.x, a.y) < tie(b.x, b.y); });
    A.erase(unique(A.begin(), A.end(), [](auto a, auto b) { return a.x == b.x && a.y == b.y; }), A.end());
    int n = A.size();
    if (n <= 2) return A;
    auto turn = [](auto a, auto b, auto c) {
        using W = conditional_t<is_integral_v<T>, i128, ld>;
        W v = (W(b.x) - a.x) * (W(c.y) - a.y) - (W(b.y) - a.y) * (W(c.x) - a.x);
        if constexpr (is_integral_v<T>) return (v > 0) - (v < 0);
        else return sign(v);
    };
    bool line = true;
    for (int i = 1; i + 1 < n; i++) if (turn(A[0], A.back(), A[i])) line = false;
    if (line) return flag ? vector<Point<T>>{A.front(), A.back()} : A;
    vector<Point<T>> h;
    auto pop = [&](auto a, auto b, auto c) { return flag ? turn(a, b, c) <= 0 : turn(a, b, c) < 0; };
    for (auto p : A) {
        while (h.size() >= 2 && pop(h[h.size() - 2], h.back(), p)) h.pop_back();
        h.push_back(p);
    }
    int k = h.size();
    for (int i = n - 2; i >= 0; --i) {
        while (h.size() > k && pop(h[h.size() - 2], h.back(), A[i])) h.pop_back();
        h.push_back(A[i]);
    }
    h.pop_back();
    return h;
}
