vector<i64> d(n + 2);
i64 last = 0;
for (int i = 1; i <= n; i++) { // 由初始数组 a 构造差分
    d[i] = a[i] - last;
    last = a[i];
}
auto rangeAdd = [&](int l, int r, i64 v) {
    d[l] += v;
    d[r + 1] -= v;
};

// 所有修改完成后还原 a
for (int i = 1; i <= n; i++) {
    d[i] += d[i - 1];
    a[i] = d[i];
}

vector<i64> pre(n + 1);
for (int i = 1; i <= n; i++) {
    pre[i] = pre[i - 1] + a[i];
}
auto rangeSum = [&](int l, int r) {
    return pre[r] - pre[l - 1];
};
