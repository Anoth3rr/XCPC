template<class T> vector<Point<T>> mincowski(vector<Point<T>> P1, vector<Point<T>> P2) {
    int n = P1.size(), m = P2.size();
    vector<Point<T>> V1(n), V2(m);
    for (int i = 0; i < n; i++) {
        V1[i] = P1[(i + 1) % n] - P1[i];
    }
    for (int i = 0; i < m; i++) {
        V2[i] = P2[(i + 1) % m] - P2[i];
    }
    vector<Point<T>> ans = {P1.front() + P2.front()};
    int t = 0, i = 0, j = 0;
    while (i < n && j < m) {
        Point<T> val = sign(cross(V1[i], V2[j])) > 0 ? V1[i++] : V2[j++];
        ans.push_back(ans.back() + val);
    }
    while (i < n) ans.push_back(ans.back() + V1[i++]);
    while (j < m) ans.push_back(ans.back() + V2[j++]);
    return ans;
}
