struct Info {
    i64 mn = numeric_limits<i64>::max();
    i64 mx = numeric_limits<i64>::lowest();

    Info() = default;
    Info(i64 x) : mn(x), mx(x) {} // 区间维护多参数时，这个构造函数允许使用简单的单值初始化叶子节点
    Info(i64 lo, i64 hi) : mn(lo), mx(hi) {} // 这个构造函数允许下面加法操作时用大括号初始化一个 Info 对象
};
Info operator+(const Info &a, const Info &b) {
    return {min(a.mn, b.mn), max(a.mx, b.mx)}; // 使用大括号初始化一个 Info 对象
}

vector<Info> a = {{}, {1}, {2}, {3}, {4}, {5}}; // 单值初始化叶子节点
