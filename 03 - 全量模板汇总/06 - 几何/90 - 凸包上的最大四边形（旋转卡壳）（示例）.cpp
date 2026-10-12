i128 maxQuadrilateralHull(const vector<P> &p) {
    int n = p.size();
    i128 ans = 0;
    for (int i = 0; i + 3 < n; i++) {
        int k = i + 1, l = i + 3;
        for (int j = i + 2; j + 1 < n; j++) {
            while (k + 1 < j && area2(p[i], p[j], p[k+1]) >= area2(p[i], p[j], p[k])) ++k;
            l = max(l, j + 1);
            while (l + 1 < n && area2(p[i], p[j], p[l+1]) >= area2(p[i], p[j], p[l])) ++l;
            ans = max(ans, area2(p[i], p[j], p[k]) + area2(p[i], p[j], p[l]));
        }
    }
    return ans;
}
