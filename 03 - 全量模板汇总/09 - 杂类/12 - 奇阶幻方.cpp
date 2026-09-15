/*
用途：Siamese 法构造 n 为正奇数的标准幻方，使用 1..n^2。
性质：每行、每列、两条主对角线之和均为 n(n^2+1)/2；O(n^2)。
*/
vector<vector<int>> magic(int n) {
    assert(n > 0 && (n & 1));
    vector a(n, vector<int>(n));
    int x = 0, y = n / 2;
    for (int z = 1; z <= n * n; ++z) {
        a[x][y] = z;
        int nx = (x + n - 1) % n, ny = (y + 1) % n;
        if (a[nx][ny]) x = (x + 1) % n;
        else x = nx, y = ny;
    }
    return a;
}
