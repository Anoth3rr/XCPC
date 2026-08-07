/*
用途：静态子串哈希、子串比较与 LCP。
用法：StringHash<u64> h(s, base) 使用自然溢出；StringHash<Z> 使用自动取模类型。
约定：区间为 [l,r)，T 必须支持 +、-、* 和从整数构造；哈希存在碰撞概率。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

template <class T> struct StringHash {

    int n = 0;
    T b{};
    vector<T> h, pw;

    StringHash() = default;

    template <class S> StringHash(const S& s, T x) {
        build(s, x);
    }

    template <class S, class F> StringHash(const S& s, T x, F f) {
        build(s, x, f);
    }

    template <class S> void build(const S& s, T x) {
        build(s, x, [](const auto& c) {
            return static_cast<u64>(static_cast<unsigned char>(c)) + 1;
        });
    }

    template <class S, class F> void build(const S& s, T x, F f) {
        n = s.size();
        b = x;
        h.assign(n + 1, T(0));
        pw.assign(n + 1, T(0));
        pw[0] = T(1);
        for (int i = 0; i < n; ++i) {
            pw[i + 1] = pw[i] * b;
            h[i + 1] = h[i] * b + T(f(s[i]));
        }
    }

    T ask(int l, int r) const {
        assert(0 <= l && l <= r && r <= n);
        return h[r] - h[l] * pw[r - l];
    }

    bool askEq(int l, int r, int x, int y) const {
        assert(r - l == y - x);
        return ask(l, r) == ask(x, y);
    }

    int askLCP(int l, int r) const {
        assert(0 <= l && l <= n && 0 <= r && r <= n);
        int x = 0, y = min(n - l, n - r) + 1;
        while (x + 1 < y) {
            int m = x + (y - x) / 2;
            if (askEq(l, l + m, r, r + m))
                x = m;
            else
                y = m;
        }
        return x;
    }
};
