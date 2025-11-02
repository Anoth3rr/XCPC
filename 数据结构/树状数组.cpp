template<class Int>
struct BIT {
    vector<Int> a;
    int n;

    BIT() {}
    BIT(int n_ = 0) {
        init(n_);
    }

    void init(int n_) {
        n = n_;
        a.assign(n + 1, Int{});
    }

    void add(int x, const Int &v) {
        for (; x <= n; x += x & -x) {
            a[x] += v;
        }
    }

    void add(int x, int y, Int k) {
        add(x, k), add(y + 1, -k);
    }

    Int ask(int x) {
        Int ans = 0;
        for (; x; x -= x & -x) {
            ans += a[x];
        }
        return ans;
    }

    Int ask(int x, int y) {
        return ask(y) - ask(x - 1);
    }

    int kth(Int k) const {
        if (k <= Int{}) return 1;
        int pos = 0;
        Int cur = Int{};
        int bit = 1;
        while ((bit << 1) <= n) bit <<= 1;
        for (; bit; bit >>= 1) {
            int nxt = pos + bit;
            if (nxt <= n && cur + a[nxt] < k) {
                cur += a[nxt];
                pos = nxt;
            }
        }
        return pos + 1;
    }
};
