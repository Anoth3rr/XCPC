void dfs(int u) {
    int t = dsu.time(); // 保存一次快照
    if (op[u].type == 0)  dsu.merge(op[u].x, op[u].y);
    if (op[u].type == 1)  ans[u] = dsu.same(op[u].x, op[u].y);
    for (int v : son[u]) {
        dfs(v);
    }
    dsu.revert(t); // 恢复到所保存的快照
}
