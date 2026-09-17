struct Rand {
    mt19937 g;
    Rand(int s = chrono::steady_clock::now().time_since_epoch().count()) : g(s) {}

    template <class T> T r(T l, T r) {
        return uniform_int_distribution<T>(l, r)(g);
    }

    template <class T> void mix(vector<T> &a) {
        shuffle(a.begin(), a.end(), g);
    }

    vector<int> perm(int n, int s = 1) {
        vector<int> a(n);
        iota(a.begin(), a.end(), s);
        mix(a);
        return a;
    }

    vector<pii> tree(int n) {
        auto p = perm(n);
        vector<E> e;
        for (int i = 1; i < n; ++i) e.push_back({p[num<int>(0, i - 1)], p[i]});
        mix(e);
        return e;
    }

    vector<pii> graph(int n, int m, bool con = true) {
        int lim = 1LL * n * (n - 1) / 2;
        vector<E> e = con ? tree(n) : vector<E>{};
        unordered_set<int> st;
        auto key = [n](int u, int v) {
            if (u > v) swap(u, v);
            return u * (n + 1) + v;
        };
        for (auto [u, v] : e) st.insert(key(u, v));
        int nd = m - e.size(), lf = lim - st.size();
        if (2LL * nd > lf) {
            vector<E> a;
            for (int u = 1; u <= n; ++u)
                for (int v = u + 1; v <= n; ++v)
                    if (!st.count(key(u, v))) a.push_back({u, v});
            mix(a);
            e.insert(e.end(), a.begin(), a.begin() + nd);
        } else {
            while (nd) {
                int u = num<int>(1, n), v = num<int>(1, n);
                if (u == v) continue;
                if (u > v) swap(u, v);
                if (st.insert(key(u, v)).second) e.push_back({u, v}), --nd;
            }
        }
        mix(e);
        return e;
    }

    string str(int n, const string &a = "abcdefghijklmnopqrstuvwxyz") {
        string s(n, ' ');
        for (char &c : s) c = a[num<int>(0, a.size() - 1)];
        return s;
    }
};