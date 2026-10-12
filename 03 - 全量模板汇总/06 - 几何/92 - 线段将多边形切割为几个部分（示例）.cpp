int cutPieces(const vector<P> &p, P a, P b) {
    auto opposite = [](i128 x, i128 y) { return (x < 0 && y > 0) || (x > 0 && y < 0); };
    int cnt = 0, n = p.size();
    for (int i = 0; i < n; i++) {
        P u = p[i], v = p[(i+1) % n];
        cnt += opposite(turn(a,b,u), turn(a,b,v)) && opposite(turn(u,v,a), turn(u,v,b));
    }
    return cnt / 2 + 1;
}
