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

bool check(int x, int y, int l1 = 0, int l2 = 0, int r1 = n, int r2 = m) {
    return l1 <= x && x <= r1 && l2 <= y && y <= r2;
};

// 进制hash
auto encode = [&](int t, auto... args) {
    ll res = 0;
    ((res = res * t + args), ...);
    return res;
};

constexpr int inf = (ll)1e9 + 7;
constexpr ll INF = (ll)2e18 + 9;
constexpr ll INF = (ll)4e18;

constexpr ll MOD = 1e9 + 7;
constexpr ll MOD = 998244353;

constexpr ld eps = 1e-10;

// horse-左上起逆时针
int dx[8] = {-1, -2, -2, -1, 1, 2, 2, 1},
    dy[8] = {2, 1, -1, -2, -2, -1, 1, 2};

// 4-左起逆时针
int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, -1, 0, 1};

// 8-左上起逆时针
int dx[8] = {-1, -1, -1, 0, 1, 1, 1, 0}, dy[8] = {1, 0, -1, -1, -1, 0, 1, 1};
