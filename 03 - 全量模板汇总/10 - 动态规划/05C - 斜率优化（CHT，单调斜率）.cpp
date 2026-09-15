/*
用途：维护 y=m*x+b 的下包络，适合 DP 转移 f[j]=min_i(m_i*x_j+b_i)。
前提：add 的斜率严格递减；ask 可任意顺序，askMono 的 x 必须非递减。
接口：add(m,b)，ask(x) 返回最小值，askMono(x) 为均摊 O(1) 查询。
复杂度：插入均摊 O(1)，ask O(log n)，askMono 均摊 O(1)。比较使用 i128；答案须落在 int。
*/
using i128 = __int128_t;

struct CHT {
    static constexpr int INF = numeric_limits<int>::max();

    struct Line {
        int m, b;

        i128 val(int x) const {
            return i128(m) * x + b;
        }
    };

    vector<Line> q;
    int p = 0, lx = numeric_limits<int>::lowest();

    static bool bad(const Line& a, const Line& b, const Line& c) {
        return (i128(b.b) - a.b) * (b.m - c.m) >= (i128(c.b) - b.b) * (a.m - b.m);
    }

    void add(int m, int b) {
        Line f{m, b};
        if (!q.empty() && q.back().m == m) {
            if (q.back().b <= b) return;
            q.pop_back();
        }
        assert(q.empty() || q.back().m > m);
        while (q.size() >= 2 && bad(q[q.size() - 2], q.back(), f)) q.pop_back();
        q.push_back(f);
        int z = q.size();
        if (p >= z) p = z - 1;
    }

    int ask(int x) const {
        assert(!q.empty());
        int l = 0, r = q.size() - 1;
        while (l < r) {
            int m = (l + r) >> 1;
            if (q[m].val(x) <= q[m + 1].val(x)) r = m;
            else l = m + 1;
        }
        i128 ans = q[l].val(x);
        assert(numeric_limits<int>::lowest() <= ans && ans <= numeric_limits<int>::max());
        return ans;
    }

    int askMono(int x) {
        assert(!q.empty() && lx <= x);
        lx = x;
        int z = q.size();
        if (p >= z) p = z - 1;
        while (p + 1 < z && q[p + 1].val(x) <= q[p].val(x)) ++p;
        i128 ans = q[p].val(x);
        assert(numeric_limits<int>::lowest() <= ans && ans <= numeric_limits<int>::max());
        return ans;
    }
};
