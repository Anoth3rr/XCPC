bigint mulSimple(const bigint &x, const bigint &y) {
    if (x.isZero() || y.isZero()) return 0;
    bigint r;
    r.sign = x.sign * y.sign;
    r.a.assign(x.a.size() + y.a.size(), 0);
    for (int i = 0; i < x.a.size(); i++) {
        i64 c = 0;
        for (int j = 0; j < y.a.size() || c; j++) {
            i64 v = r.a[i + j] + c;
            if (j < y.a.size()) v += x.a[i] * y.a[j];
            r.a[i + j] = v % base, c = v / base;
        }
    }
    r.trim();
    return r;
}
