struct Tag {
    i64 add = 0;
    void apply(const Tag &v) { add += v.add; }
};
struct Info {
    i64 sum = 0;
    i64 mn = numeric_limits<i64>::max();
    i64 mx = numeric_limits<i64>::lowest();

    Info() = default;
    Info(i64 x) : sum(x), mn(x), mx(x) {}
    Info(i64 s, i64 lo, i64 hi) : sum(s), mn(lo), mx(hi) {}
    void apply(const Tag &v, int len) {
        sum += v.add * len;
        mn += v.add;
        mx += v.add;
    }
};
Info operator+(const Info &a, const Info &b) {
    return {a.sum + b.sum, min(a.mn, b.mn), max(a.mx, b.mx)};
}
