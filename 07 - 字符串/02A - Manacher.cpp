/*
用途：回文半径、区间回文判定与最长回文。
模板开关：Manacher<true, false> 只求偶回文；Manacher<false, true> 只求奇回文。
约定：半径数组使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <bool E = true, bool O = true> struct Manacher {

    int n = 0;
    vector<int> o, e;

    Manacher() = default;

    template <class A> Manacher(int n, A a) {
        init(n, a);
    }

    template <class It> Manacher(It l, It r) {
        init(r - l, [&](int i) { return *(l + i); });
    }

    template <class S> explicit Manacher(const S& s) : Manacher(s.begin(), s.end()) {}

    template <class A> void init(int m, A a) {
        assert(m >= 0);
        n = m;
        if constexpr (O) {
            o.assign(n, 0);
            for (int l = 0, r = -1, i = 0; i < n; ++i) {
                int k = i <= r ? min(o[l + r - i], r - i) + 1 : 1;
                while (k <= i && k < n - i && a(i - k) == a(i + k)) ++k;
                o[i] = k - 1;
                if (i + k - 1 > r) l = i - k + 1, r = i + k - 1;
            }
        } else {
            o.clear();
        }
        if constexpr (E) {
            e.assign(n, 0);
            for (int l = 0, r = -1, i = 0; i < n; ++i) {
                int k = i <= r ? min(e[l + r - i + 1], r - i + 1) + 1 : 1;
                while (k <= i && k < n - i + 1 && a(i - k) == a(i + k - 1)) ++k;
                e[i] = k - 1;
                if (i + k - 2 > r) l = i - k + 1, r = i + k - 2;
            }
        } else {
            e.clear();
        }
    }

    template <bool Odd> int askRad(int x) const {
        assert(0 <= x && x < n);
        if constexpr (Odd) {
            static_assert(O, "odd radii are disabled");
            return o[x];
        } else {
            static_assert(E, "even radii are disabled");
            return e[x];
        }
    }

    template <bool Odd> bool askPal(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        if constexpr (Odd) {
            static_assert(O, "odd radii are disabled");
            assert(!((l ^ r) & 1));
            return o[(l + r) >> 1] >= (r - l) / 2;
        } else {
            static_assert(E, "even radii are disabled");
            assert((l ^ r) & 1);
            return e[(l + r + 1) >> 1] > (r - l) / 2;
        }
    }

    bool ask(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        if ((l ^ r) & 1) {
            if constexpr (E) return askPal<false>(l, r);
            assert(false);
            return false;
        }
        if constexpr (O) return askPal<true>(l, r);
        assert(false);
        return false;
    }

    array<int, 2> askMax() const {
        array<int, 2> a{0, 0};
        if constexpr (O) {
            for (int i = 0; i < n; ++i) {
                array<int, 2> b{i - o[i], i + o[i] + 1};
                if (b[1] - b[0] > a[1] - a[0]) a = b;
            }
        }
        if constexpr (E) {
            for (int i = 0; i < n; ++i) {
                array<int, 2> b{i - e[i], i + e[i]};
                if (b[1] - b[0] > a[1] - a[0]) a = b;
            }
        }
        return a;
    }

    int askLen() const {
        auto a = askMax();
        return a[1] - a[0];
    }
};
