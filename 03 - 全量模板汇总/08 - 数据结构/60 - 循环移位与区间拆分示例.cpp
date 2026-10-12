cin >> l >> r >> x;
l--; // 坐标修改为 0 开始
r--;
l = (l + shift) % n; // 偏移
r = (r + shift) % n;
if (l > r) { // 区间分离则分别操作
    segt.modify(l, n - 1, x);
    segt.modify(0, r, x);
} else {
    segt.modify(l, r, x);
}
