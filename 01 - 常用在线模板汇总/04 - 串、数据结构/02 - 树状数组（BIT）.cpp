template <class T> struct BIT {
    int n{};
    vector<T> w;

    BIT() {}
    BIT(int _n) : n(_n), w(_n + 1) {}

    void add(int x, const T &k) {
        for (; x <= n; x += x & -x) {
            w[x] += k;
        }
    }
    void add(int x, int y, const T &k) {
        add(x, k), add(y, -k);
    }
    T ask(int x) const {
        T ans = 0;
        for (; x; x -= x & -x) {
            ans += w[x];
        }
        return ans;
    }
    T ask(int x, int y) const {
        return ask(y) - ask(x - 1);
    }
    int kth(const T &k) const {
        int pos = 0;
        T pre{};
        int stp = 1;
        while ((stp << 1) <= n) stp <<= 1;
        for (; stp; stp >>= 1) {
            int nxt = pos + stp;
            if (nxt <= n && pre + w[nxt] < k) {
                pre += w[nxt];
                pos = nxt;
            }
        }
        return pos + 1;
    }
    int get(auto val) {
        this->n = val.size() - 1;
        w.assign(n + 1, T{});

        vector<pii> alls;
        for (int i = 1; i <= n; i++) {
            alls.emplace_back(val[i], i);
        }
        sort(alls.begin(), alls.end());

        int ans = 0;
        for (auto [val, idx] : alls) {
            ans += ask(idx + 1, n);
            add(idx, 1);
        }
        return ans;
    }
};
