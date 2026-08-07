/*
用途：只关注区间第 k 小/第 k 大的轻量 Wavelet Matrix 封装。

区间和 k 均为 0-indexed 闭区间：askKth(l,r,k) 是第 k 小，askKth<true>(l,r,k) 是第 k 大。
构造后静态查询，预处理 O(n log sigma)、查询 O(log sigma)。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

using u64 = uint64_t;

template <typename T> class Wavelet {
  private:
    struct Core {
        struct BitVec {
            static constexpr int W = 64;
            static int ask64(u64 x, int i) {
                return __builtin_popcountll(x & ((1ull << i) - 1));
            }
#pragma pack(4)
            struct Block {
                u64 bit;
                int sum;
            };
#pragma pack()
            int n, z;
            vector<Block> b;
            BitVec(int _n = 0) : n(_n), b(n / W + 1) {}
            int operator[](int i) const {
                return b[i / W].bit >> i % W & 1;
            }
            void modify(int i) {
                b[i / W].bit |= u64(1) << i % W;
            }
            void build() {
                for (int j = 0; j < n / W; ++j) b[j + 1].sum = b[j].sum + __builtin_popcountll(b[j].bit);
                z = ask0(n);
            }
            int ask0(int i) const {
                return i - ask1(i);
            }
            int ask1(int i) const {
                auto &&e = b[i / W];
                return e.sum + ask64(e.bit, i % W);
            }
        };

        int n = 0, lg = 0;
        vector<T> a;
        vector<BitVec> bv;
        Core(int _n = 0) : n(_n), a(n) {}
        Core(const vector<T> &_a) : n(_a.size()), a(_a) {
            build();
        }
        T &operator[](int i) {
            return a[i];
        }
        void build() {
            if (n == 0) {
                lg = 0;
                bv.clear();
                return;
            }
            lg = __lg(max<T>(*max_element(begin(a), end(a)), 1)) + 1;
            bv.assign(lg, n);
            vector<T> cur = a, nxt(n);
            for (auto h = lg; h--;) {
                for (int i = 0; i < n; ++i)
                    if (cur[i] >> h & 1) bv[h].modify(i);
                bv[h].build();
                array<typename vector<T>::iterator, 2> it{begin(nxt), begin(nxt) + bv[h].z};
                for (int i = 0; i < n; ++i) *it[bv[h][i]]++ = cur[i];
                swap(cur, nxt);
            }
        }
        T askKth(int l, int r, int k) const {
            T res = 0;
            for (auto h = lg; h--;) {
                auto l0 = bv[h].ask0(l), r0 = bv[h].ask0(r);
                if (k < r0 - l0)
                    l = l0, r = r0;
                else {
                    k -= r0 - l0;
                    res |= T{1} << h;
                    l += bv[h].z - l0;
                    r += bv[h].z - r0;
                }
            }
            return res;
        }
        int askCnt(int l, int r, T ub) const {
            if (ub >= T{1} << lg) return r - l;
            int res = 0;
            for (auto h = lg; h--;) {
                auto l0 = bv[h].ask0(l), r0 = bv[h].ask0(r);
                if (~ub >> h & 1)
                    l = l0, r = r0;
                else {
                    res += r0 - l0;
                    l += bv[h].z - l0;
                    r += bv[h].z - r0;
                }
            }
            return res;
        }
        int askCnt(int l, int r, T lb, T ub) const {
            return askCnt(l, r, ub) - askCnt(l, r, lb);
        }
    };
    pair<vector<int>, vector<T>> zip(const vector<T> &a) {
        int n = a.size();
        vector<pair<T, int>> p(n);
        for (int i = 0; i < n; ++i) p[i] = {a[i], i};
        sort(begin(p), end(p));
        vector<int> na(n);
        vector<T> v;
        for (int k = 0, rnk = -1; k < n; ++k) {
            if (k == 0 or p[k - 1].first < p[k].first) v.push_back(p[k].first), ++rnk;
            na[p[k].second] = rnk;
        }
        return make_pair(na, v);
    }
    void check(int l, int r, int k) const {
        assert(l >= 0 && l <= r && r < n && k <= r - l && k >= 0);
    }
    vector<T> vec;
    Core wm;
    int n = 0;

  public:
    Wavelet() {}
    Wavelet(const vector<T> &v) : n(v.size()) {
        auto [a, _v] = zip(v);
        wm = Core(a);
        vec = _v;
    }
    template <bool Rev = false> T askKth(int l, int r, int k) const {
        check(l, r, k);
        if constexpr (Rev) k = r - l - k;
        r++;
        return vec[wm.askKth(l, r, k)];
    }
};
