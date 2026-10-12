constexpr int B = 31; // 保证所有数都在 [0, 2^B) 内

struct Tag {
    i64 mask = 0;
    void apply(const Tag &v) { mask ^= v.mask; }
};
struct Info {
    array<int, B> cnt{};

    Info() = default;
    Info(i64 x) {
        for (int i = 0; i < B; i++) {
            cnt[i] = x >> i & 1;
        }
    }
    void apply(const Tag &v, int len) {
        for (int i = 0; i < B; i++) {
            if (v.mask >> i & 1) {
                cnt[i] = len - cnt[i];
            }
        }
    }
    i64 sum() const {
        i64 res = 0;
        for (int i = 0; i < B; i++) {
            res += 1LL * cnt[i] * (1LL << i);
        }
        return res;
    }
};
Info operator+(const Info &a, const Info &b) {
    Info c;
    for (int i = 0; i < B; i++) {
        c.cnt[i] = a.cnt[i] + b.cnt[i];
    }
    return c;
}

vector<Info> a = {{}, {1}, {2}, {3}, {4}, {5}};
LazySegTree<Info, Tag> t(a);

t.rangeApply(2, 4, {3}); // [1, 1, 0, 7, 5]

cout << t.rangeQuery(1, 5).sum() << "\n"; // 14
