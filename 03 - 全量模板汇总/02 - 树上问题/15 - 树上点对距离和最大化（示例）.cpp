vector<int> val(n + 1, 1);
int ans = 0;
function<void(int, int)> dfs = [&](int x, int fa) {
    for (auto y : ver[x]) {
        if (y == fa) continue;
        dfs(y, x);
        val[x] += val[y];
        ans += min(val[y], k - val[y]);
    }
};
dfs(1, 0);
cout << ans << endl;
