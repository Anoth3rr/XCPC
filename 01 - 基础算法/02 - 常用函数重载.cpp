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

template <typename Int>
Int gcd(Int a, Int b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

template <typename Int>
Int lcm(Int a, Int b) {
    return a / gcd(a, b) * b;
}

template <typename Int>
bool In(Int x, Int a, Int b) {
    return min(a, b) <= x && x <= max(a, b);
}

auto check = [&](int x, int y, int l1 = 0, int l2 = 0, int r1 = n, int r2 = m) {
    return l1 <= x && x <= r1 && l2 <= y && y <= r2;
};

// 进制hash
auto encode = [&](int t, auto... args) {
    ll res = 0;
    ((res = res * t + args), ...);
    return res;
};