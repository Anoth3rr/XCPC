exBIT bit(n);
for (int x = 1; x <= n; x++) {
    bit.add(hld.in[x], hld.in[x], w[x]);
}

auto nodeAdd = [&](int x, i64 v) { // 节点 x 修改
    bit.add(hld.in[x], hld.in[x], v);
};
auto pathSum = [&](int x, int y) { // 节点 x 到 y 的路径和查询
    i64 ans = 0;
    hld.forPath(x, y, [&](int l, int r) {
        ans += bit.ask(l, r);
    });
    return ans;
};
auto subtreeSum = [&](int x) { // 节点 x 子树和查询
    auto [l, r] = hld.subtree(x);
    return bit.ask(l, r);
};
auto pathAdd = [&](int x, int y, i64 v) { // 节点 x 到 y 的路径修改
    hld.forPath(x, y, [&](int l, int r) {
        bit.add(l, r, v);
    });
};
auto subtreeAdd = [&](int x, i64 v) { // 节点 x 子树修改
    auto [l, r] = hld.subtree(x);
    bit.add(l, r, v);
};
