vector<int> phiDivisors(int n) {
    vector<int> phi(n + 1);
    iota(phi.begin(), phi.end(), 0LL);
    for (int i = 1; i <= n; i++)
        for (int j = i + i; j <= n; j += i) phi[j] -= phi[i];
    return phi;
}
