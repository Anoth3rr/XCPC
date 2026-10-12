array<int, 3> maxTriangle(const vector<P> &p) {
    int n = p.size();
    if (n < 3) return {-1, -1, -1};
    array<int, 3> ans{0, 1, 2};
    i128 best = area2(p[0], p[1], p[2]);
    for (int i = 0; i + 2 < n; i++) {
        int k = i + 2;
        for (int j = i + 1; j + 1 < n; j++) {
            k = max(k, j + 1);
            while (k + 1 < n && area2(p[i], p[j], p[k+1]) >= area2(p[i], p[j], p[k])) ++k;
            i128 s = area2(p[i], p[j], p[k]);
            if (s > best) best = s, ans = {i, j, k};
        }
    }
    return ans;
}
