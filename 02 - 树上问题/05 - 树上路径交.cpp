/*
用途：求两条树上简单路径的公共点数。

依赖：
- askLCA(u, v)：返回最近公共祖先。
- askDis(u, v)：返回 u 到 v 的边数距离。

接口：ask(a, b, c, d) 返回路径 a-b 与 c-d 的公共点个数，
不相交时返回 0。时间复杂度 O(log n)，常数很小。
*/
bool askOn(int a, int b, int x) {
    return askDis(a, x) + askDis(x, b) == askDis(a, b);
}

int ask(int a, int b, int c, int d) {
    vector<int> pnt = {a, b, c, d};
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            pnt.push_back(askLCA(pnt[i], pnt[j]));
        }
    }
    sort(pnt.begin(), pnt.end());
    pnt.erase(unique(pnt.begin(), pnt.end()), pnt.end());

    vector<int> g;
    for (int x : pnt) {
        if (askOn(a, b, x) && askOn(c, d, x)) g.push_back(x);
    }
    if (g.empty()) return 0;

    int ans = 1;
    for (int x : g) {
        for (int y : g) {
            ans = max(ans, askDis(x, y) + 1);
        }
    }
    return ans;
}
