/*
用途：带下界/上界的可行循环流，以及带源汇的最大流。
依赖：01A - 最大流（Dinic）。
约定：点编号 0..n-1，add(u,v,lo,hi) 满足 0 <= lo <= hi。
ask 求可行循环流；flow(s,t) 求满足上下界后的最大 S-T 流。
复杂度：常数次 Dinic 最大流。
*/

template <class Cap = int> struct BoundFlow {
    static constexpr Cap INF = numeric_limits<Cap>::max() / 4;

    struct Edge {
        int u, v;
        Cap lo, hi;
    };

    struct Result {
        bool ok;
        Cap val;          // 循环流时为 0；S-T 流时为最大流值。
        vector<Cap> a;    // 与 add 顺序对应，已包含下界。
    };

    int n;
    vector<Edge> e;

    explicit BoundFlow(int n) : n(n) {}

    int add(int u, int v, Cap lo, Cap hi) {
        assert(0 <= u && u < n && 0 <= v && v < n && 0 <= lo && lo <= hi);
        e.push_back({u, v, lo, hi});
        return e.size() - 1;
    }

    Result ask() const {
        static_assert(std::is_signed_v<Cap>, "Cap must be signed because balances may be negative.");
        Network z = build(false, -1, -1);
        Cap f = z.mf.flow(z.ss, z.tt);
        if (f != z.nd) return {false, 0, {}};
        return {true, 0, get(z)};
    }

    Result flow(int s, int t) const {
        static_assert(std::is_signed_v<Cap>, "Cap must be signed because balances may be negative.");
        assert(0 <= s && s < n && 0 <= t && t < n && s != t);
        Network z = build(true, s, t);
        Cap f = z.mf.flow(z.ss, z.tt);
        if (f != z.nd) return {false, 0, {}};

        Cap bas = z.mf.askFlow(z.rid);
        for (int id : z.aid) z.mf.erase(id);
        z.mf.erase(z.rid);
        Cap add = z.mf.flow(s, t);
        return {true, bas + add, get(z)};
    }

  private:
    struct Network {
        MaxFlow<Cap> mf;
        int ss, tt;
        Cap nd = 0;
        vector<int> eid;
        vector<int> aid;
        int rid = -1;

        explicit Network(int n) : mf(n + 2), ss(n), tt(n + 1) {}
    };

    Network build(bool bk, int s, int t) const {
        Network z(n);
        vector<Cap> bal(n, 0);
        z.eid.reserve(e.size());
        for (const Edge& x : e) {
            z.eid.push_back(z.mf.add(x.u, x.v, x.hi - x.lo));
            bal[x.u] -= x.lo;
            bal[x.v] += x.lo;
        }
        if (bk) {
            z.rid = z.mf.add(t, s, INF);
        }
        for (int u = 0; u < n; ++u) {
            if (bal[u] > 0) {
                z.aid.push_back(z.mf.add(z.ss, u, bal[u]));
                z.nd += bal[u];
            } else if (bal[u] < 0) {
                z.aid.push_back(z.mf.add(u, z.tt, -bal[u]));
            }
        }
        return z;
    }

    vector<Cap> get(const Network& z) const {
        vector<Cap> res(e.size());
        for (int id = 0; id < e.size(); ++id) {
            res[id] = e[id].lo + z.mf.askFlow(z.eid[id]);
        }
        return res;
    }
};
