int calc(int n, int m, int a, int b) {
    int cnt = n, mod = m, coe = a, off = b, ans = 0;
    while (true) {
        if (coe >= mod) {
            ans += (cnt - 1) * cnt * (coe / mod) / 2;
            coe %= mod;
        }
        if (off >= mod) {
            ans += cnt * (off / mod);
            off %= mod;
        }
        int mx = coe * cnt + off;
        if (mx < mod) break;
        cnt = mx / mod;
        off = mx % mod;
        swap(mod, coe);
    }
    return ans;
}