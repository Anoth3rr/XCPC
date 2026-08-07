/*
用途：离散化任意可比较值；下标从 0 开始。
接口：先 add / 构造，再 build；askId(x) 要求 x 已出现，ask / askR 可查询任意值。
复杂度：build O(k log k)，其余查询 O(log k)。
*/
template <class T> struct Compress {
    vector<T> a;

    Compress() = default;

    explicit Compress(vector<T> a) : a(move(a)) {
        build();
    }

    void add(const T& x) {
        a.push_back(x);
    }

    template <class It> void add(It l, It r) {
        a.insert(a.end(), l, r);
    }

    void build() {
        sort(a.begin(), a.end());
        a.erase(unique(a.begin(), a.end()), a.end());
    }

    int askSz() const {
        return a.size();
    }

    int ask(const T& x) const {
        return lower_bound(a.begin(), a.end(), x) - a.begin();
    }

    int askR(const T& x) const {
        return upper_bound(a.begin(), a.end(), x) - a.begin();
    }

    int askId(const T& x) const {
        int p = ask(x);
        assert(p < askSz() && a[p] == x);
        return p;
    }

    const T& operator[](int p) const {
        assert(0 <= p && p < askSz());
        return a[p];
    }
};
