i128 maxQuadrilateral(const vector<P> &a) {
    assert(a.size() >= 4);
    auto h = convexHullInt(a);
    if (h.size() < 3) return 0;
    if (h.size() >= 4) return maxQuadrilateralHull(h);
    array<int, 3> used{};
    i128 ans = 0, s = area2(h[0], h[1], h[2]);
    for (auto p : a) {
        bool skip = false;
        for (int i = 0; i < 3; i++) {
            if (!used[i] && p.x == h[i].x && p.y == h[i].y) {
                used[i] = 1, skip = true;
                break;
            }
        }
        if (skip) continue; // 每个凸包顶点仅消耗一个输入编号，重复点仍可作第四点
        i128 cut = min({area2(p, h[0], h[1]), area2(p, h[1], h[2]), area2(p, h[2], h[0])});
        ans = max(ans, s - cut);
    }
    return ans;
}
