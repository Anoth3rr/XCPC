/*
适用范围：有限、无环、无偏（双方可走的边相同）、正常游戏规则（不能走者负）。
Nim：异或和非 0 为先手必胜；misere 单独处理“最后一步输”的反常 Nim。
SG：多个独立子游戏的 Grundy 值异或；buildSG 要求所有转移均指向更小编号状态。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

namespace Game {

inline bool nim(const vector<int>& pil) {
    int xs = 0;
    for (int pil : pil) {
        assert(pil >= 0);
        xs ^= pil;
    }
    return xs != 0;
}

// 返回 {堆下标, 操作后剩余石子数}；必败态返回 nullopt。
inline optional<pair<int, int>> nimMove(const vector<int>& a) {
    int x = 0;
    for (int v : a) assert(v >= 0), x ^= v;
    if (!x) return nullopt;
    for (int i = 0; i < a.size(); ++i) {
        int v = a[i] ^ x;
        if (v < a[i]) return pair<int, int>{i, v};
    }
    return nullopt;
}

inline bool bash(int n, int m) {
    assert(n >= 0 && m > 0);
    return n % (m + 1) != 0;
}

// 每次可同时修改至多 k 堆；P 态当且仅当每一二进制位的 1 数量均为 k+1 的倍数。
inline bool moore(const vector<int>& a, int k) {
    assert(k >= 1);
    for (int b = 0; b < 64; ++b) {
        int c = 0;
        for (auto x : a) c += x >> b & 1;
        if (c % (k + 1)) return true;
    }
    return false;
}

// a[0] 表示第 1 级台阶；只异或奇数级台阶。
inline bool stair(const vector<int>& a) {
    int x = 0;
    for (int i = 0; i < a.size(); i += 2) assert(a[i] >= 0), x ^= a[i];
    return x != 0;
}

inline bool misere(const vector<int>& pil) {
    bool one = true;
    int cnt = 0;
    int xs = 0;
    for (int pil : pil) {
        assert(pil >= 0);
        one &= pil <= 1;
        cnt += pil > 0;
        xs ^= pil;
    }
    return one ? (cnt % 2 == 0) : (xs != 0);
}

inline int mex(const vector<int>& a) {
    vector<char> vis(a.size() + 1);
    for (int x : a) if (0 <= x && x <= a.size()) vis[x] = true;
    for (int x = 0;; ++x) if (!vis[x]) return x;
}

template <class F> vector<int> buildSG(int n, F to) {
    assert(n >= 0);
    vector<int> sg(n);
    for (int s = 0; s < n; ++s) {
        vector<int> a;
        for (int v : to(s)) {
            assert(0 <= v && v < s);
            a.push_back(sg[v]);
        }
        sg[s] = mex(a);
    }
    return sg;
}

inline vector<int> subSG(int n, vector<int> mv) {
    assert(n >= 0);
    for (int x : mv) assert(x > 0);
    sort(mv.begin(), mv.end());
    mv.erase(unique(mv.begin(), mv.end()), mv.end());
    return buildSG(n + 1, [&](int s) {
        vector<int> a;
        for (int x : mv) {
            if (x > s) break;
            a.push_back(s - x);
        }
        return a;
    });
}

inline bool sumSG(const vector<int>& a) {
    int x = 0;
    for (int v : a) {
        assert(v >= 0);
        x ^= v;
    }
    return x != 0;
}

inline bool antiSG(const vector<int>& a) {
    int x = 0, big = 0;
    for (int v : a) assert(v >= 0), x ^= v, big += v > 1;
    return big ? x != 0 : x == 0;
}

inline int lasker(int x) {
    assert(x >= 0);
    if (!x) return 0;
    return x % 4 == 0 ? x - 1 : x % 4 == 3 ? x + 1 : x;
}

inline int treeSG(const vector<vector<int>>& g, int rt = 1) {
    auto dfs = [&](auto&& go, int u, int fa) -> int {
        int x = 0;
        for (int v : g[u]) if (v != fa) x ^= go(go, v, u) + 1;
        return x;
    };
    return dfs(dfs, rt, -1);
}

inline bool fib(int n) {
    if (n <= 1) return false;
    int a = 1, b = 2;
    while (b < n) {
        if (numeric_limits<int>::max() - b < a) return true;
        auto c = a + b;
        a = b, b = c;
    }
    return b != n;
}
}  // namespace Game
