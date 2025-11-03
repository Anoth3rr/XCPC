template<class Int>
struct BIT {
    vector<Int> a;
    int n;

    BIT() {}
    BIT(int n) {
        init(n);
    }

    void init(int n) {
        this->n = n;
        a.resize(n + 1);
    }

    void add(int x, int k) {
        for (; x <= n; x += x & -x) {
            a[x] += k;
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

    Int kth(int k) {
        Int ans = 0;
        for (int i = __lg(n); i >= 0; i--) {
            Int val = ans + (1 << i);
            if (val < n && a[val] < k) {
                k -= a[val];
                ans = val;
            }
        }
        return ans + 1;
    }
};