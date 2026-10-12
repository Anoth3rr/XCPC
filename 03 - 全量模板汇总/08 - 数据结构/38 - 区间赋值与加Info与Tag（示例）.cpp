struct Tag {
    bool f = false;
    i64 set = 0, add = 0;
    void apply(const Tag &v) { // 先执行本标记，再执行 v
        if (v.f) {
            f = true;
            set = v.set;
            add = v.add;
        } else {
            add += v.add;
        }
    }
};
struct Info {
    i64 sum = 0;
    i64 mn = numeric_limits<i64>::max();
    i64 mx = numeric_limits<i64>::lowest();

    Info() = default;
    Info(i64 x) : sum(x), mn(x), mx(x) {}
    Info(i64 s, i64 lo, i64 hi) : sum(s), mn(lo), mx(hi) {}
    void apply(const Tag &v, int len) {
        if (v.f) {
            sum = v.set * len;
            mn = mx = v.set;
        }
        sum += v.add * len;
        mn += v.add;
        mx += v.add;
    }
};
Info operator+(const Info &a, const Info &b) {
    return {a.sum + b.sum, min(a.mn, b.mn), max(a.mx, b.mx)};
}

vector<Info> a = {{}, {5}, {1}, {7}, {3}, {4}};
LazySegTree<Info, Tag> t(a);

t.rangeApply(2, 4, {true, 6, 0});   // 执行区间赋值：[5, 6, 6, 6, 4]
t.rangeApply(3, 5, {false, 0, -2}); // 不执行区间赋值：[5, 6, 4, 4, 2]

auto ans = t.rangeQuery(1, 5);
cout << ans.sum << " " << ans.mn << " " << ans.mx << "\n"; // 21 2 6
