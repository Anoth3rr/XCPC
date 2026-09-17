/*
用途：64 位异或线性基：插入、可表示性、最大异或、化简后的基向量。
复杂度：每次插入/查询 O(64)，化简 O(64^2)。可合并、可持久化等扩展见数据结构目录的线性基。
*/

using u64 = uint64_t;

struct LB {
    array<u64, 64> a{};
    int rk = 0;

    bool insert(u64 val) {
        for (int bit = 63; bit >= 0; --bit) {
            if (((val >> bit) & 1ULL) == 0) continue;
            if (a[bit] == 0) {
                a[bit] = val;
                ++rk;
                return true;
            }
            val ^= a[bit];
        }
        return false;
    }

    bool ask(u64 val) const {
        for (int bit = 63; bit >= 0; --bit) {
            if (((val >> bit) & 1ULL) == 0) continue;
            if (a[bit] == 0) return false;
            val ^= a[bit];
        }
        return true;
    }

    u64 askMax(u64 sd = 0) const {
        u64 ans = sd;
        for (int bit = 63; bit >= 0; --bit) {
            if ((ans ^ a[bit]) > ans) ans ^= a[bit];
        }
        return ans;
    }

    void reduce() {
        for (int hi = 63; hi >= 0; --hi) {
            if (a[hi] == 0) continue;
            for (int low = hi - 1; low >= 0; --low) {
                if (a[low] != 0 && ((a[hi] >> low) & 1ULL)) a[hi] ^= a[low];
            }
        }
    }

    vector<u64> askVec() const {
        LB z = *this;
        z.reduce();
        vector<u64> res;
        for (int bit = 0; bit < 64; ++bit)
            if (z.a[bit] != 0) res.push_back(z.a[bit]);
        return res;
    }
};
