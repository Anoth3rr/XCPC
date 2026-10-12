void update(int p, int l, int r, int ql, int qr, int mod) {
    if (qr < l || r < ql || tr[p].val < mod) return;
    if (l == r) return tr[p].val %= mod, void();

    int m = (l + r) >> 1;
    update(p << 1, l, m, ql, qr, mod);
    update(p << 1 | 1, m + 1, r, ql, qr, mod);
    tr[p] = tr[p << 1] + tr[p << 1 | 1];
}
void update(int l, int r, int mod) {
    if (l > r) return;
    update(1, 1, n, l, r, mod);
}
