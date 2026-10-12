Z Lagrange(const vector<array<Z, 2>> &f, Z x) {
    int n = f.size();
    Z ans = 0;
    for (int i = 0; i < n; i++) {
        Z a = f[i][1], b = 1;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            a *= x - f[j][0];
            b *= f[i][0] - f[j][0];
        }
        ans += a / b;
    }
    return ans;
}
