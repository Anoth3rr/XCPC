struct Info {
    int sum{};
    int p = -inf, s = -inf, mx = -inf;

    Info() = default;
    Info(int x) : sum(x), p(x), s(x), mx(x) {}
    Info(int s, int l, int r, int m) : sum(s), p(l), s(r), mx(m) {}

    Info operator+(const Info &o) {
        return {sum + o.sum, max(p, sum + o.p), max(o.s, o.sum + s), max({mx, o.mx, s + o.p})};
    }
};

vector<Info> a = {{}, {-2}, {3}, {-1}, {4}, {-5}};
SegTree tr(5);
tr.build(a);

cout << tr.ask(1, 5).mx << endl; // 6
tr.modify(1, {5});
cout << tr.ask(1, 5).mx << endl; // 11
