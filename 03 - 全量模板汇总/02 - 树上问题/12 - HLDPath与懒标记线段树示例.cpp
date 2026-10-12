vector<Info> a(n + 1);
for (int x = 1; x <= n; x++) {
    a[hld.in[x]] = Info(w[x]);
}
LazySegTree<Info, Tag> seg(a); // 要求 Info 可按任意顺序合并

auto pathApply = [&](int x, int y, const Tag &v) { // 节点 x 到 y 的路径修改
    hld.forPath(x, y, [&](int l, int r) {
        seg.rangeApply(l, r, v);
    });
};
auto subtreeApply = [&](int x, const Tag &v) { // 节点 x 子树修改
    auto [l, r] = hld.subtree(x);
    seg.rangeApply(l, r, v);
};
auto pathQuery = [&](int x, int y) { // 节点 x 到 y 的路径和查询
    Info ans;
    hld.forPath(x, y, [&](int l, int r) {
        ans = ans + seg.rangeQuery(l, r);
    });
    return ans;
};

seg.pointSet(hld.in[x], Info(v)); // 点 x 赋值为 v
