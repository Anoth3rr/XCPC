template <class Int>
struct WaveletMatrix {
    using ull = unsigned long long;
    struct BitVector {
        int n;
        vector<ull> bits;
        vector<int> pref;

        BitVector() : n(0) {}
        BitVector(int n_) { init(n_); }

        void init(int n_) {
            n = n_;
            int blocks = (n >> 6) + 1;
            bits.assign(blocks, 0);
            pref.assign(blocks + 1, 0);
        }

        inline void set(int i) { bits[i >> 6] |= (1ull << (i & 63)); }

        void build() {
            for (int i = 0; i < bits.size(); ++i) {
                pref[i + 1] = pref[i] + __builtin_popcountll(bits[i]);
            }
        }

        inline int rank1(int pos) const { return pref[pos >> 6] + __builtin_popcountll(bits[pos >> 6] & ((1ull << (pos & 63)) - 1)); }

        inline int rank0(int pos) const { return pos - rank1(pos); }

        inline int access(int i) const { return (bits[i >> 6] >> (i & 63)) & 1; }
    };

    int n, bitlen;
    vector<BitVector> mats;
    vector<int> mids;
    vector<Int> vals;

    WaveletMatrix() : n(0), bitlen(0) {}

    WaveletMatrix(vector<Int> data) { build(data); }

    void build(vector<Int> data) {
        n = data.size();
        vals = data;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        int sig = vals.size();
        if (sig == 0) {
            bitlen = 0;
            return;
        }

        vector<int> code(n);
        for (int i = 0; i < n; ++i) {
            code[i] = lower_bound(vals.begin(), vals.end(), data[i]) - vals.begin();
        }

        bitlen = 0;
        while ((1ll << bitlen) < sig) ++bitlen;
        if (bitlen == 0) bitlen = 1;

        mats.assign(bitlen, BitVector());
        mids.assign(bitlen, 0);

        for (int level = 0; level < bitlen; ++level) {
            mats[level].init(n);
        }

        vector<int> next(n);
        for (int level = bitlen - 1; level >= 0; --level) {
            auto& bv = mats[level];
            for (int i = 0; i < n; ++i) {
                if ((code[i] >> level) & 1) bv.set(i);
            }
            bv.build();

            int zcount = 0;
            for (int i = 0; i < n; ++i) {
                if (!((code[i] >> level) & 1)) {
                    next[zcount++] = code[i];
                }
            }
            int idx = zcount;
            for (int i = 0; i < n; ++i) {
                if ((code[i] >> level) & 1) {
                    next[idx++] = code[i];
                }
            }
            mids[level] = zcount;
            code.swap(next);
        }
    }

    inline int size() const { return n; }
    inline bool empty() const { return n == 0; }

    Int access(int pos) const {
        assert(pos >= 0 && pos < n);
        int code = 0;
        for (int level = bitlen - 1; level >= 0; --level) {
            int b = mats[level].access(pos);
            if (b) {
                code |= (1 << level);
                pos = mids[level] + mats[level].rank1(pos);
            } else {
                pos = mats[level].rank0(pos);
            }
        }
        return vals[code];
    }

    Int kth(int l, int r, int k) const {
        assert(l >= 0 && l <= r && r <= n && k >= 0 && k < r - l);
        int code = 0;
        for (int level = bitlen - 1; level >= 0; --level) {
            const BitVector& bv = mats[level];
            int zleft = bv.rank0(l);
            int zright = bv.rank0(r);
            int zeros = zright - zleft;
            if (k < zeros) {
                l = zleft;
                r = zright;
            } else {
                k -= zeros;
                code |= (1 << level);
                l = mids[level] + bv.rank1(l);
                r = mids[level] + bv.rank1(r);
            }
        }
        return vals[code];
    }

    int countle(int l, int r, Int x) const {
        if (l >= r || vals.empty()) return 0;
        auto it = lower_bound(vals.begin(), vals.end(), x);
        if (it == vals.end()) return r - l;
        int xcode = it - vals.begin();
        if (xcode == 0) return 0;
        int res = 0;
        for (int level = bitlen - 1; level >= 0; --level) {
            const BitVector& bv = mats[level];
            int zleft = bv.rank0(l), zright = bv.rank0(r);
            if ((xcode >> level) & 1) {
                res += zright - zleft;
                l = mids[level] + bv.rank1(l);
                r = mids[level] + bv.rank1(r);
            } else {
                l = zleft;
                r = zright;
            }
        }
        return res;
    }

    int countleq(int l, int r, Int x) const {
        if (l >= r || vals.empty()) return 0;
        auto it = upper_bound(vals.begin(), vals.end(), x);
        if (it == vals.end()) return r - l;
        int xcode = it - vals.begin();
        if (xcode == 0) return 0;
        int res = 0;
        for (int level = bitlen - 1; level >= 0; --level) {
            const BitVector& bv = mats[level];
            int zleft = bv.rank0(l), zright = bv.rank0(r);
            if ((xcode >> level) & 1) {
                res += zright - zleft;
                l = mids[level] + bv.rank1(l);
                r = mids[level] + bv.rank1(r);
            } else {
                l = zleft;
                r = zright;
            }
        }
        return res;
    }

    int range(int l, int r, Int Min, Int Max) const {
        if (Min > Max) return 0;
        return countleq(l, r, Max) - countle(l, r, Min);
    }

    int freq(int l, int r, Int x) const {
        if (l >= r || vals.empty()) return 0;
        auto it = lower_bound(vals.begin(), vals.end(), x);
        if (it == vals.end() || *it != x) return 0;
        int code = it - vals.begin();
        for (int level = bitlen - 1; level >= 0; --level) {
            const BitVector& bv = mats[level];
            if (!((code >> level) & 1)) {
                l = bv.rank0(l);
                r = bv.rank0(r);
            } else {
                l = mids[level] + bv.rank1(l);
                r = mids[level] + bv.rank1(r);
            }
        }
        return r - l;
    }
};