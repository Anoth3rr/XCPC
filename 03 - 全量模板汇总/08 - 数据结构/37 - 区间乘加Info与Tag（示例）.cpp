i64 MOD = 1'000'000'007; // 建树和操作前赋为题目给定模数

struct Tag {
    i64 mul = 1, add = 0;
    void apply(const Tag &v) { // 先执行本标记，再执行 v
        add = (add * v.mul + v.add) % MOD; // 注意先后顺序
        mul = mul * v.mul % MOD;
    }
};
struct Info {
    i64 sum = 0;
    void apply(const Tag &v, int len) {
        sum = (sum * v.mul + v.add * len) % MOD;
    }
};
Info operator+(const Info &a, const Info &b) {
    return {(a.sum + b.sum) % MOD};
}

vector<Info> a = {{}, {1}, {2}, {3}, {4}, {5}};
LazySegTree<Info, Tag> t(a);

t.rangeApply(2, 4, {2, 0}); // x <- 2 * x：[1, 4, 6, 8, 5]
t.rangeApply(3, 5, {1, 3}); // x <- 1 * x + 3：[1, 4, 9, 11, 8]

cout << t.rangeQuery(2, 5).sum << "\n"; // 32
