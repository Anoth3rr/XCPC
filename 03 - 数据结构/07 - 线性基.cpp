template <class Int>
struct LB {
    using ull = unsigned long long;
    static constexpr int BASE = sizeof(Int) * 8 - 1;
    vector<Int> d, p;
    int cnt, flag;
    vector<ull> mask;
    vector<int> rep;

    LB() {
        d.assign(BASE + 1, 0);
        p.assign(BASE + 1, 0);
        mask.assign(BASE + 1, 0);
        cnt = flag = 0;
    }
    bool insert(Int val, int idx) {
        ull cur = 0;
        for (int i = BASE - 1; i >= 0; i--) {
            if (val & (1ll << i)) {
                if (!d[i]) {
                    d[i] = val;
                    mask[i] = cur | (1ull << rep.size());
                    rep.push_back(idx);
                    return true;
                }
                val ^= d[i];
                cur ^= mask[i];
            }
        }
        flag = 1;
        return false;
    }
    pair<bool, ull> check(Int val) {
        ull res = 0;
        for (int i = BASE - 1; i >= 0; --i) {
            if (val & (1ll << i)) {
                if (!d[i])
                    return {false, 0ull};
                val ^= d[i];
                res ^= mask[i];
            }
        }
        return {true, res};
    }
    Int ask_max() {
        Int res = 0;
        for (int i = BASE - 1; i >= 0; i--) {
            if ((res ^ d[i]) > res)
                res ^= d[i];
        }
        return res;
    }
    Int ask_min() {
        if (flag)
            return 0;
        for (int i = 0; i <= BASE - 1; i++) {
            if (d[i])
                return d[i];
        }
    }
    void rebuild() {
        for (int i = BASE - 1; i >= 0; i--) {
            for (int j = i - 1; j >= 0; j--) {
                if (d[i] & (1ll << j))
                    d[i] ^= d[j];
            }
        }
        for (int i = 0; i <= BASE - 1; i++) {
            if (d[i])
                p[cnt++] = d[i];
        }
    }
    Int kthquery(ull k) {
        if (flag)
            k--;
        if (!k)
            return 0;
        Int res = 0;
        if (k >= (1ll << cnt))
            return -1;
        for (int i = BASE - 1; i >= 0; i--) {
            if (k & (1LL << i))
                res ^= p[i];
        }
        return res;
    }
    void Merge(const LB &b) {
        for (int i = BASE - 1; i >= 0; i--) {
            if (b.d[i]) {
                insert(b.d[i]);
            }
        }
    }
    vector<int> choose(ull mask) {
        vector<int> res;
        for (int i = 0; i < rep.size(); ++i) {
            if (mask & (1ull << i)) {
                res.pb(rep[i]);
            }
        }
        return res;
    }
};