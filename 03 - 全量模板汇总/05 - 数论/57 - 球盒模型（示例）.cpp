i64 mypow(i64 n, i64 k) { // 复杂度是 log N
    i64 r = 1;
    for (; k; k >>= 1, n *= n) {
        if (k & 1) r *= n;
    }
    return r;
}
 
vector<vector<i64>> comb;
void YangHuiTriangle(int n = 60) {
    comb.resize(n + 1, vector<i64>(n + 1));
    comb[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        comb[i][0] = 1;
        for (int j = 1; j <= n; j++) {
            comb[i][j] = comb[i - 1][j] + comb[i - 1][j - 1];
        }
    }
}
 
vector<vector<i64>> S;
void Stirling2(int n = 15) {
    S.resize(n + 1, vector<i64>(n + 1));
    S[1][1] = 1;
    for (int i = 2; i <= 15; i++) {
        for (int j = 1; j <= i; j++) {
            S[i][j] = S[i - 1][j - 1] + S[i - 1][j] * j;
        }
    }
}
 
vector<vector<i64>> dp;
void GeneratingFunction(int n = 15) {
    dp.resize(n + 1, vector<i64>(n + 1));
    for (int i = 0; i <= n; i++) {
        dp[i][1] = 1;
        for (int j = 2; j <= n; j++) {
            dp[i][j] = dp[i][j - 1];
            if (i >= j) dp[i][j] += dp[i - j][j];
        }
    }
}
 
vector<i64> fac;
void Fac(int n = 30) {
    fac.resize(n + 1);
    fac[0] = 1;
    for (int i = 1; i <= n; i++) {
        fac[i] = fac[i - 1] * i;
    }
}
 
i64 A(int n, int m) {
    if (n < 0 || m < 0 || n < m) return 0;
    return fac[n] / fac[n - m];
}
 
i64 C(int n, int m) {
    if (n < 0 || m < 0 || n < m) return 0;
    return comb[n][m];
}
 
signed main() {
    int Task = 1;
    for (cin >> Task; Task; Task--) {
        int op, n, m;
        cin >> op >> n >> m;
 
        i64 ans = -1;
        if (op == 1) { // 球同，盒同、能空
            ans = dp[n][m];
        } else if (op == 2) { // 球同，盒同、至多放一个
            ans = (n <= m);
        } else if (op == 3) { // 球同，盒同、至少放一个
            ans = (n < m ? 0 : dp[n - m][m]);
        } else if (op == 4) { // 球同，盒不同、能空
            ans = C(m - 1 + n, n);
        } else if (op == 5) { // 球同，盒不同、至多放一个
            ans = C(m, n);
        } else if (op == 6) { // 球同，盒不同、至少放一个
            ans = C(n - 1, m - 1);
        } else if (op == 7) { // 球不同，盒同、能空
            ans = accumulate(S[n].begin() + 1, S[n].begin() + m + 1, 0LL);
        } else if (op == 8) { // 球不同，盒同、至多放一个
            ans = (n <= m);
        } else if (op == 9) { // 球不同，盒同、至少放一个
            ans = S[n][m];
        } else if (op == 10) { // 球不同，盒不同、能空
            ans = mypow(m, n);
        } else if (op == 11) { // 球不同，盒不同、至多放一个
            ans = A(m, n);
        } else if (op == 12) { // 球不同，盒不同、至少放一个
            ans = fac[m] * S[n][m];
        }
        cout << ans << "\n";
    }
}
