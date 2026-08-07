/*
用途：用排序 vector 模拟有序多重集；接口与常见平衡树题一致。
环境：查询 O(log n)，插入/删除 O(n)；n 较小、修改少或只想极短实现时使用。
*/
template <class T> struct OSet {
    vector<T> a;

    int askSz() const { return a.size(); }
    bool askEmp() const { return a.empty(); }
    void clear() { a.clear(); }

    void insert(const T &x) { a.insert(lower_bound(a.begin(), a.end(), x), x); }

    bool erase(const T &x) {
        auto p = lower_bound(a.begin(), a.end(), x);
        if (p == a.end() || *p != x) return false;
        a.erase(p);
        return true;
    }

    int askRank(const T &x) const { return lower_bound(a.begin(), a.end(), x) - a.begin() + 1; }
    int askCnt(const T &x) const { return upper_bound(a.begin(), a.end(), x) - lower_bound(a.begin(), a.end(), x); }

    optional<T> askKth(int k) const {
        if (k < 1 || k > askSz()) return nullopt;
        return a[k - 1];
    }

    optional<T> askPre(const T &x) const {
        auto p = lower_bound(a.begin(), a.end(), x);
        return p == a.begin() ? nullopt : optional<T>(*--p);
    }

    optional<T> askNxt(const T &x) const {
        auto p = upper_bound(a.begin(), a.end(), x);
        return p == a.end() ? nullopt : optional<T>(*p);
    }
};
