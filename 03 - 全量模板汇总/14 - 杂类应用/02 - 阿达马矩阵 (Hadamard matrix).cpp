int n;
cin >> n;
int N = pow(2, n);
vector ans(N, vector<int>(N));
ans[0][0] = 1;
for (int t = 0; t < n; t++) {
    int m = pow(2, t);
    for (int i = 0; i < m; i++) {
        for (int j = m; j < 2 * m; j++) {
            ans[i][j] = ans[i][j - m];
        }
    }
    for (int i = m; i < 2 * m; i++) {
        for (int j = 0; j < m; j++) {
            ans[i][j] = ans[i - m][j];
        }
    }
    for (int i = m; i < 2 * m; i++) {
        for (int j = m; j < 2 * m; j++) {
            ans[i][j] = 1 - ans[i - m][j - m];
        }
    }
}
