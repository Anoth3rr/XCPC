/*
用途：判定查询序列是否为固定主序列的子序列；支持任意可排序元素类型。
预处理 O(n log sigma)，单次查询 O(m(log sigma+log n))，空间 O(n)。
askPos 返回匹配最后一个元素的 0-indexed 位置；空序列返回 -1。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class Cmp = less<T>> struct SubSeq {
    Cmp cmp;
    vector<T> val;
    vector<vector<int>> pos;

    SubSeq() = default;
    template <class Seq> explicit SubSeq(const Seq& a) { build(a); }

    template <class Seq> void build(const Seq& a) {
        val.assign(a.begin(), a.end());
        sort(val.begin(), val.end(), cmp);
        auto eq = [&](const T& x, const T& y) { return !cmp(x, y) && !cmp(y, x); };
        val.erase(unique(val.begin(), val.end(), eq), val.end());
        pos.assign(val.size(), {});
        int i = 0;
        for (const auto& x : a) {
            int k = lower_bound(val.begin(), val.end(), x, cmp) - val.begin();
            pos[k].push_back(i++);
        }
    }

    template <class Seq> optional<int> askPos(const Seq& a) const {
        int at = -1;
        for (const auto& x : a) {
            auto p = lower_bound(val.begin(), val.end(), x, cmp);
            if (p == val.end() || cmp(x, *p) || cmp(*p, x)) return nullopt;
            int k = p - val.begin();
            auto it = upper_bound(pos[k].begin(), pos[k].end(), at);
            if (it == pos[k].end()) return nullopt;
            at = *it;
        }
        return at;
    }

    template <class Seq> bool ask(const Seq& a) const { return askPos(a).has_value(); }
};
