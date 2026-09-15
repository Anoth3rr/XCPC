/*
用途：整数横坐标 0..n-1 上的 Newton 前向差分级数。
构造 O(n^2)，单次 ask O(n)；Z 为质数模且 n < mod。
*/
template <class Z> struct Newton {
    vector<Z> d;

    Newton() = default;

    explicit Newton(vector<Z> a) {
        init(move(a));
    }

    void init(vector<Z> a) {
        assert(a.size() < Z::askMod());
        d.clear();
        while (!a.empty()) {
            d.push_back(a[0]);
            for (int i = 0; i + 1 < a.size(); ++i) a[i] = a[i + 1] - a[i];
            a.pop_back();
        }
    }

    Z ask(Z x) const {
        Z r = 0, c = 1;
        for (int i = 0; i < d.size(); ++i) {
            r += d[i] * c;
            c *= (x - Z(i)) / Z(i + 1);
        }
        return r;
    }
};
