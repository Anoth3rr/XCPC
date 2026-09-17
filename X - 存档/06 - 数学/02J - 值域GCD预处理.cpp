/*
用途：预处理后快速求 gcd(x,y)，其中 0 <= x <= n，y 可为 64 位整数。
复杂度：预处理 O(n + sqrt(n)^2)，单次 O(1)。默认 n=1e6。
限制：适合 x 在固定小值域内、y 很大且查询很多的场景；普通单次 gcd 不要使用。
性能：因 fac/gd 是大表，纯下标和表项必须保留 i32；改为宏展开后的 int 会显著增加内存。
*/

using i32 = int32_t;

struct FastGCD {
    i32 n, b;
    vector<i32> p;
    vector<bool> ip;
    vector<array<i32, 3>> fac;
    vector<vector<i32>> gd;

    explicit FastGCD(i32 m = 1'000'000) {
        init(m);
    }

    void init(i32 m) {
        assert(m >= 1);
        n = m;
        b = 1;
        while (1LL * (b + 1) * (b + 1) <= n) ++b;
        gd.assign(b + 1, vector<i32>(b + 1));
        for (i32 i = 0; i <= b; ++i) gd[i][0] = gd[0][i] = i;
        for (i32 i = 1; i <= b; ++i)
            for (i32 j = i; j <= b; ++j) gd[i][j] = gd[j][i] = gd[i][j % i];

        p.clear();
        ip.assign(n + 1, true);
        ip[0] = false;
        if (n >= 1) ip[1] = false;
        fac.assign(n + 1, {});
        fac[1] = {1, 1, 1};
        for (i32 i = 2; i <= n; ++i) {
            if (ip[i]) {
                p.push_back(i);
                fac[i] = {1, 1, i};
            }
            for (i32 q : p) {
                if (i > n / q) break;
                i32 v = i * q;
                ip[v] = false;
                fac[v] = fac[i];
                *min_element(fac[v].begin(), fac[v].end()) *= q;
                if (i % q == 0) break;
            }
        }
    }

    i32 ask0(i32 x, int y) const {
        i32 r = y % x;
        return ip[x] ? (r ? 1 : x) : gd[r][x];
    }

    int ask(i32 x, int y) const {
        if (!x) return y < 0 ? -y : y;
        assert(x <= n);
        if (y < 0) y = -y;
        i32 a = ask0(fac[x][0], y);
        y /= a;
        i32 b = ask0(fac[x][1], y);
        y /= b;
        i32 c = ask0(fac[x][2], y);
        return 1LL * a * b * c;
    }
};
