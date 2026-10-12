constexpr int N = 1e5 + 7;

vector<vector<int>> f(N + 1);

void init() {
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= N / i; ++j)
            f[i * j].push_back(i);
}
