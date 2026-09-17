constexpr int N = 110;
constexpr double eps = 1e-8;
int n;
double a[N][N];
int gauss() {
    int c, r;
    for (c = 0, r = 0; c < n; c++) {
        int t = r;
        for (int i = r; i < n; ++i)
            if (fabs(a[i][c]) > fabs(a[t][c])) t = i;
        if (fabs(a[t][c]) < eps) continue;
        for (int j = c; j < n + 1; ++j) swap(a[t][j], a[r][j]);
        for (int j = n; j >= c; --j) a[r][j] /= a[r][c];
        for (int i = r + 1; i < n; ++i)
            if (fabs(a[i][c]) > eps)
                for (int j = n; j >= c; --j) a[i][j] -= a[r][j] * a[i][c];
        r++;
    }
    if (r < n) {
        for (int i = r; i < n; ++i)
            if (fabs(a[i][n]) > eps) return 2;
        return 1;
    }
    for (int i = n - 1; i >= 0; --i)
        for (int j = i + 1; j < n; ++j) a[i][n] -= a[i][j] * a[j][n];
    return 0;
}
int main() {
    cin >> n;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n + 1; ++j) cin >> a[i][j];
    int t = gauss();
    if (t == 0) {
        for (int i = 0; i < n; ++i) {
            if (fabs(a[i][n]) < eps) a[i][n] = abs(a[i][n]);
            cout << a[i][n] << endl;
        }
    } else if (t == 1)
        cout << "Infinite group solutions" << endl;
    else
        cout << "No solution" << endl;
    return 0;
}