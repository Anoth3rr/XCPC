/*
用途：对拍时生成随机数、排列、树、简单连通无向图和字符串。
约定：点编号 1..n；图无自环、无重边。稠密图改为枚举补边，避免拒绝采样卡死。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct Gen {
    using E = array<int, 2>;
    mt19937_64 g;

    explicit Gen(int s = chrono::steady_clock::now().time_since_epoch().count()) : g(s) {}

    template <class T> T num(T l, T r) {
        assert(l <= r);
        return uniform_int_distribution<T>(l, r)(g);
    }

    template <class T> void mix(vector<T>& a) {
        shuffle(a.begin(), a.end(), g);
    }

    vector<int> perm(int n, int s = 1) {
        assert(n >= 0);
        vector<int> a(n);
        iota(a.begin(), a.end(), s);
        mix(a);
        return a;
    }

    vector<E> tree(int n) {
        assert(n >= 1);
        auto p = perm(n);
        vector<E> e;
        for (int i = 1; i < n; ++i) e.push_back({p[num<int>(0, i - 1)], p[i]});
        mix(e);
        return e;
    }

    vector<E> graph(int n, int m, bool con = true) {
        assert(n >= 1);
        int lim = n * (n - 1) / 2;
        assert(0 <= m && m <= lim && (!con || m >= n - 1));
        vector<E> e = con ? tree(n) : vector<E>{};
        unordered_set<int> st;
        auto key = [n](int u, int v) {
            if (u > v) swap(u, v);
            return u * (n + 1) + v;
        };
        for (auto [u, v] : e) st.insert(key(u, v));
        int nd = m - e.size(), lf = lim - st.size();
        if (nd * 2 > lf) {
            vector<E> a;
            for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
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

    string str(int n, string_view a = "abcdefghijklmnopqrstuvwxyz") {
        assert(n >= 0 && !a.empty());
        string s(n, ' ');
        for (char& c : s) c = a[num<int>(0, a.size() - 1)];
        return s;
    }
};
