i128 maxQuadrilateralBrute(const vector<P> &a) {
    i128 ans = 0;
    for (int i = 0; i < a.size(); i++) {
        for (int j = i + 1; j < a.size(); j++) {
            i128 l = 0, r = 0;
            for (auto p : a) {
                i128 v = turn(a[i], a[j], p);
                if (v > 0) l = max(l, v);
                else r = max(r, -v);
            }
            if (l && r) ans = max(ans, l + r);
        }
    }
    return ans;
}
