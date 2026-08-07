/*
用途：压缩值域 Wavelet Matrix，支持 0-indexed 半开区间 [l, r) 的访问、k 小、计数和频率查询。
askLT/askLE 分别表示 < x / <= x；askCnt(l,r,L,R) 统计 [L,R] 内元素个数。
预处理 O(n log sigma)，单次查询 O(log sigma)。
性能：层前缀、位置和压缩编号默认使用 int；极限内存不足时可只将这些纯计数与下标改为 i32。
*/

using u64 = uint64_t;

template <class T> struct Wavelet {
    struct BitVec {
        int n;
        vector<u64> bit;
        vector<int> pre;

        BitVec() : n(0) {}
        BitVec(int _n) {
            init(_n);
        }

        void init(int _n) {
            n = _n;
            int blk = (n >> 6) + 1;
            bit.assign(blk, 0);
            pre.assign(blk + 1, 0);
        }

        inline void modify(int i) {
            bit[i >> 6] |= (1ull << (i & 63));
        }

        void build() {
            for (int i = 0; i < bit.size(); ++i) {
                pre[i + 1] = pre[i] + __builtin_popcountll(bit[i]);
            }
        }

        inline int ask1(int pos) const {
            return pre[pos >> 6] + __builtin_popcountll(bit[pos >> 6] & ((1ull << (pos & 63)) - 1));
        }

        inline int ask0(int pos) const {
            return pos - ask1(pos);
        }

        inline int ask(int i) const {
            return (bit[i >> 6] >> (i & 63)) & 1;
        }
    };

    int n, len;
    vector<BitVec> mat;
    vector<int> mid;
    vector<T> val;

    Wavelet() : n(0), len(0) {}

    Wavelet(vector<T> dat) {
        build(dat);
    }

    void build(vector<T> dat) {
        n = dat.size();
        val = dat;
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());
        int sig = val.size();
        if (sig == 0) {
            len = 0;
            mat.clear();
            mid.clear();
            return;
        }

        vector<int> cod(n);
        for (int i = 0; i < n; ++i) {
            cod[i] = lower_bound(val.begin(), val.end(), dat[i]) - val.begin();
        }

        len = 0;
        while ((1ll << len) < sig) ++len;
        if (len == 0) len = 1;

        mat.assign(len, BitVec());
        mid.assign(len, 0);

        for (int lev = 0; lev < len; ++lev) {
            mat[lev].init(n);
        }

        vector<int> nxt(n);
        for (int lev = len - 1; lev >= 0; --lev) {
            auto &bv = mat[lev];
            for (int i = 0; i < n; ++i) {
                if ((cod[i] >> lev) & 1) bv.modify(i);
            }
            bv.build();

            int cnt = 0;
            for (int i = 0; i < n; ++i) {
                if (!((cod[i] >> lev) & 1)) {
                    nxt[cnt++] = cod[i];
                }
            }
            int idx = cnt;
            for (int i = 0; i < n; ++i) {
                if ((cod[i] >> lev) & 1) {
                    nxt[idx++] = cod[i];
                }
            }
            mid[lev] = cnt;
            cod.swap(nxt);
        }
    }

    inline int askSz() const {
        return n;
    }
    inline bool askEmp() const {
        return n == 0;
    }

    T ask(int pos) const {
        assert(pos >= 0 && pos < n);
        int cod = 0;
        for (int lev = len - 1; lev >= 0; --lev) {
            int b = mat[lev].ask(pos);
            if (b) {
                cod |= (1 << lev);
                pos = mid[lev] + mat[lev].ask1(pos);
            } else {
                pos = mat[lev].ask0(pos);
            }
        }
        return val[cod];
    }

    T askKth(int l, int r, int k) const {
        assert(l >= 0 && l <= r && r <= n && k >= 0 && k < r - l);
        int cod = 0;
        for (int lev = len - 1; lev >= 0; --lev) {
            const BitVec &bv = mat[lev];
            int zl = bv.ask0(l);
            int zr = bv.ask0(r);
            int zs = zr - zl;
            if (k < zs) {
                l = zl;
                r = zr;
            } else {
                k -= zs;
                cod |= (1 << lev);
                l = mid[lev] + bv.ask1(l);
                r = mid[lev] + bv.ask1(r);
            }
        }
        return val[cod];
    }

    int askLT(int l, int r, T x) const {
        if (l >= r || val.empty()) return 0;
        auto it = lower_bound(val.begin(), val.end(), x);
        if (it == val.end()) return r - l;
        int xc = it - val.begin();
        if (xc == 0) return 0;
        int res = 0;
        for (int lev = len - 1; lev >= 0; --lev) {
            const BitVec &bv = mat[lev];
            int zl = bv.ask0(l), zr = bv.ask0(r);
            if ((xc >> lev) & 1) {
                res += zr - zl;
                l = mid[lev] + bv.ask1(l);
                r = mid[lev] + bv.ask1(r);
            } else {
                l = zl;
                r = zr;
            }
        }
        return res;
    }

    int askLE(int l, int r, T x) const {
        if (l >= r || val.empty()) return 0;
        auto it = upper_bound(val.begin(), val.end(), x);
        if (it == val.end()) return r - l;
        int xc = it - val.begin();
        if (xc == 0) return 0;
        int res = 0;
        for (int lev = len - 1; lev >= 0; --lev) {
            const BitVec &bv = mat[lev];
            int zl = bv.ask0(l), zr = bv.ask0(r);
            if ((xc >> lev) & 1) {
                res += zr - zl;
                l = mid[lev] + bv.ask1(l);
                r = mid[lev] + bv.ask1(r);
            } else {
                l = zl;
                r = zr;
            }
        }
        return res;
    }

    int askCnt(int l, int r, T mn, T mx) const {
        if (mn > mx) return 0;
        return askLE(l, r, mx) - askLT(l, r, mn);
    }

    int askFreq(int l, int r, T x) const {
        if (l >= r || val.empty()) return 0;
        auto it = lower_bound(val.begin(), val.end(), x);
        if (it == val.end() || *it != x) return 0;
        int cod = it - val.begin();
        for (int lev = len - 1; lev >= 0; --lev) {
            const BitVec &bv = mat[lev];
            if (!((cod >> lev) & 1)) {
                l = bv.ask0(l);
                r = bv.ask0(r);
            } else {
                l = mid[lev] + bv.ask1(l);
                r = mid[lev] + bv.ask1(r);
            }
        }
        return r - l;
    }
};
