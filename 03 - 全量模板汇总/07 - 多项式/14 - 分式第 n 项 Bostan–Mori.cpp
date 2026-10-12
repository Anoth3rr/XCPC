Z bostanMori(vector<Z> p, vector<Z> q, i64 n) {
    assert(n >= 0 && !q.empty() && q[0] != Z(0));
    while (n && !p.empty()) {
        auto h = q;
        for (int i = 1; i < h.size(); i += 2) h[i] = -h[i];
        p = NTT::mul(p, h), q = NTT::mul(q, h);
        vector<Z> a, b;
        for (int i = n & 1; i < p.size(); i += 2) a.push_back(p[i]);
        for (int i = 0; i < q.size(); i += 2) b.push_back(q[i]);
        p = move(a), q = move(b);
        n >>= 1;
    }
    return p.empty() ? Z(0) : p[0] / q[0];
}
