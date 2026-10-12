struct Tag {
    i64 mask = 0;
    void apply(const Tag &v) { mask ^= v.mask; }
};
struct Info {
    i64 xr = 0;
    void apply(const Tag &v, int len) {
        if (len & 1) xr ^= v.mask;
    }
};
Info operator+(const Info &a, const Info &b) {
    return {a.xr ^ b.xr};
}

vector<Info> a = {{}, {1}, {2}, {3}, {4}, {5}};
LazySegTree<Info, Tag> t(a);

t.rangeApply(2, 4, {3}); // [1, 1, 0, 7, 5]

cout << t.rangeQuery(1, 5).xr << "\n"; // 2
