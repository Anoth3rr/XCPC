/*
用途：异或线性基，支持可表示性、最大异或、最小非空异或和第 k 小非空子集异或值。

T 必须是整数类型；内部按 T 的全部二进制位工作，不会遗漏最高位。
- insert(x)：插入 x，返回是否线性无关。
- ask(x)：是否能由已插入数异或得到 x。
- askMax(seed = 0)：max(seed xor subsetXor)。
- askMin()：最小非空子集异或值；若存在非空子集异或为 0 则返回 0。
- askKth(k)：第 k 小“非空子集”异或结果（1-indexed，重复结果只计一次），不存在返回 -1。

调用 askKth/askMin 前会自动化为最简基，复杂度 O(B^2)，B 为位数（最多 64）。
*/

using u64 = uint64_t;

template <class T = u64>
struct LB {
    static_assert(is_integral_v<T>);
    using U = make_unsigned_t<T>;
    static constexpr int B = numeric_limits<U>::digits;

    array<U, B> bas{};
    vector<U> ord;
    int cnt = 0;
    bool dep = false;

    bool insert(T val) {
        U x = static_cast<U>(val);
        for (int bit = B - 1; bit >= 0; --bit) {
            if (!((x >> bit) & U(1))) continue;
            if (!bas[bit]) {
                bas[bit] = x;
                ++cnt;
                ord.clear();
                return true;
            }
            x ^= bas[bit];
        }
        dep = true;
        return false;
    }

    bool ask(T val) const {
        U x = static_cast<U>(val);
        for (int bit = B - 1; bit >= 0; --bit) {
            if (!((x >> bit) & U(1))) continue;
            if (!bas[bit]) return false;
            x ^= bas[bit];
        }
        return true;
    }

    T askMax(T sd = T{}) const {
        U ans = static_cast<U>(sd);
        for (int bit = B - 1; bit >= 0; --bit) {
            if ((ans ^ bas[bit]) > ans) ans ^= bas[bit];
        }
        return static_cast<T>(ans);
    }

    void rebuild() {
        for (int hi = B - 1; hi >= 0; --hi) {
            if (!bas[hi]) continue;
            for (int low = hi - 1; low >= 0; --low) {
                if ((bas[hi] >> low) & U(1)) bas[hi] ^= bas[low];
            }
        }
        ord.clear();
        for (int bit = 0; bit < B; ++bit) {
            if (bas[bit]) ord.push_back(bas[bit]);
        }
    }

    T askMin() {
        if (dep) return T{};
        rebuild();
        return ord.empty() ? T{} : static_cast<T>(ord.front());
    }

    T askKth(u64 k) {
        if (k == 0) return T(-1);
        rebuild();
        u64 msk;
        if (dep) {
            msk = k - 1;  // 非空子集也可得到 0。
        } else {
            msk = k;  // 0 对应空集，非空结果从 mask = 1 开始。
        }
        if (cnt < 64 && msk >= (1ull << cnt)) return T(-1);
        U ans = 0;
        for (int i = 0; i < cnt; ++i) {
            if ((msk >> i) & 1ull) ans ^= ord[i];
        }
        return static_cast<T>(ans);
    }

    void merge(const LB &o) {
        for (int bit = B - 1; bit >= 0; --bit) {
            if (o.bas[bit]) insert(static_cast<T>(o.bas[bit]));
        }
        dep = dep || o.dep;
    }
};
