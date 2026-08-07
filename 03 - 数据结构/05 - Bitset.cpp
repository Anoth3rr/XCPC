/*
用途：运行时长度 Bitset，支持位运算、移位和枚举置位。

下标为 0..askSz()-1；askNxt/askPre 找不到时返回 askSz()。
resize(n, true) 会将“新增的位”置 1，行为与 vector 的扩容语义一致。
*/

using u64 = uint64_t;

class Bitset {
  public:
    static constexpr int W = 64;

  private:
    vector<u64> a;
    int nb = 0;
    u64 msk = ~u64(0);

    static int wds(int bit) {
        if (bit < 0) throw invalid_argument("negative bitset size");
        return (bit + W - 1) / W;
    }

    void trim() {
        int rem = nb % W;
        msk = rem == 0 ? ~u64(0) : (u64(1) << rem) - 1;
        if (!a.empty()) a.back() &= msk;
    }

    void check(int pos) const {
        if (pos < 0 || pos >= nb) throw out_of_range("bit index out of range");
    }

  public:
    Bitset() = default;
    explicit Bitset(int n, bool val = false) {
        resize(n, val);
    }

    int askSz() const noexcept {
        return nb;
    }

    bool askEmp() const noexcept {
        return nb == 0;
    }

    void reset() noexcept {
        fill(all(a), 0);
    }

    void set() noexcept {
        fill(all(a), ~u64(0));
        if (!a.empty()) a.back() &= msk;
    }

    void resize(int n, bool val = false) {
        int nw = wds(n);
        int ob = nb;
        if (n == ob) return;

        if (n > ob) {
            a.resize(nw, val ? ~u64(0) : 0);
            if (val) {
                int fw = ob / W;
                int fb = ob % W;
                if (fw < nw) {
                    if (fb) a[fw] |= (~u64(0) << fb);
                    else a[fw] = ~u64(0);
                    for (int i = fw + 1; i < nw; ++i) a[i] = ~u64(0);
                }
            }
        } else {
            a.resize(nw);
        }
        nb = n;
        trim();
    }

    void modify(int pos, bool v) {
        check(pos);
        u64 b = u64(1) << (pos % W);
        if (v) a[pos / W] |= b;
        else a[pos / W] &= ~b;
    }

    void flip(int pos) {
        check(pos);
        a[pos / W] ^= u64(1) << (pos % W);
    }

    bool ask(int pos) const {
        check(pos);
        return (a[pos / W] >> (pos % W)) & 1;
    }

    bool askAny() const noexcept {
        for (u64 wd : a) {
            if (wd) return true;
        }
        return false;
    }

    bool askNone() const noexcept {
        return !askAny();
    }

    bool askAll() const noexcept {
        if (a.empty()) return true;
        for (int i = 0; i + 1 < a.size(); ++i) {
            if (a[i] != ~u64(0)) return false;
        }
        return a.back() == msk;
    }

    int askCnt() const noexcept {
        int ans = 0;
        for (u64 wd : a) ans += __builtin_popcountll(wd);
        return ans;
    }

    Bitset &operator&=(const Bitset &o) {
        assert(nb == o.nb);
        for (int i = 0; i < a.size(); ++i) a[i] &= o.a[i];
        return *this;
    }

    Bitset &operator|=(const Bitset &o) {
        assert(nb == o.nb);
        for (int i = 0; i < a.size(); ++i) a[i] |= o.a[i];
        if (!a.empty()) a.back() &= msk;
        return *this;
    }

    Bitset &operator^=(const Bitset &o) {
        assert(nb == o.nb);
        for (int i = 0; i < a.size(); ++i) a[i] ^= o.a[i];
        if (!a.empty()) a.back() &= msk;
        return *this;
    }

    Bitset operator~() const {
        Bitset res(*this);
        for (u64 &wd : res.a) wd = ~wd;
        if (!res.a.empty()) res.a.back() &= res.msk;
        return res;
    }

    friend Bitset operator&(Bitset lhs, const Bitset &rhs) {
        return lhs &= rhs;
    }

    friend Bitset operator|(Bitset lhs, const Bitset &rhs) {
        return lhs |= rhs;
    }

    friend Bitset operator^(Bitset lhs, const Bitset &rhs) {
        return lhs ^= rhs;
    }

    Bitset &operator<<=(int sh) {
        if (sh < 0) throw invalid_argument("negative bit shift");
        if (sh == 0 || nb == 0) return *this;
        int sw = sh / W;
        int sb = sh % W;
        int wds = a.size();
        if (sw >= wds) {
            reset();
            return *this;
        }
        if (sb == 0) {
            for (int i = wds - 1; i >= sw; --i) a[i] = a[i - sw];
        } else {
            for (int i = wds - 1; i >= sw; --i) {
                u64 hi = a[i - sw] << sb;
                u64 low = i - sw > 0 ? a[i - sw - 1] >> (W - sb) : 0;
                a[i] = hi | low;
            }
        }
        for (int i = 0; i < sw; ++i) a[i] = 0;
        a.back() &= msk;
        return *this;
    }

    Bitset &operator>>=(int sh) {
        if (sh < 0) throw invalid_argument("negative bit shift");
        if (sh == 0 || nb == 0) return *this;
        int sw = sh / W;
        int sb = sh % W;
        int wds = a.size();
        if (sw >= wds) {
            reset();
            return *this;
        }
        if (sb == 0) {
            for (int i = 0; i + sw < wds; ++i) a[i] = a[i + sw];
        } else {
            for (int i = 0; i + sw < wds; ++i) {
                u64 low = a[i + sw] >> sb;
                u64 hi = i + sw + 1 < wds ? a[i + sw + 1] << (W - sb) : 0;
                a[i] = low | hi;
            }
        }
        for (int i = wds - sw; i < wds; ++i) a[i] = 0;
        a.back() &= msk;
        return *this;
    }

    friend Bitset operator<<(Bitset lhs, int sh) {
        return lhs <<= sh;
    }

    friend Bitset operator>>(Bitset lhs, int sh) {
        return lhs >>= sh;
    }

    int askNxt(int pos) const noexcept {
        if (pos < 0) pos = 0;
        if (pos >= nb) return nb;
        int id = pos / W;
        u64 wd = a[id] & (~u64(0) << (pos % W));
        if (wd) return id * W + __builtin_ctzll(wd);
        for (++id; id < a.size(); ++id) {
            if (a[id]) return id * W + __builtin_ctzll(a[id]);
        }
        return nb;
    }

    int askPre(int pos) const noexcept {
        if (pos < 0 || nb == 0) return nb;
        if (pos >= nb) pos = nb - 1;
        int id = pos / W;
        int off = pos % W;
        u64 msk = off == 63 ? ~u64(0) : (u64(1) << (off + 1)) - 1;
        u64 wd = a[id] & msk;
        if (wd) return id * W + 63 - __builtin_clzll(wd);
        while (id > 0) {
            --id;
            if (a[id]) return id * W + 63 - __builtin_clzll(a[id]);
        }
        return nb;
    }

    template <class F>
    void each(F f) const {
        for (int i = 0; i < a.size(); ++i) {
            u64 wd = a[i];
            while (wd) {
                int bit = __builtin_ctzll(wd);
                f(i * W + bit);
                wd &= wd - 1;
            }
        }
    }

    string str() const {
        string res;
        res.reserve(nb);
        for (int i = nb - 1; i >= 0; --i) res.push_back(ask(i) ? '1' : '0');
        return res;
    }

    const u64 *askData() const noexcept {
        return a.empty() ? nullptr : a.data();
    }

    u64 *askData() noexcept {
        return a.empty() ? nullptr : a.data();
    }

    static Bitset read(const string &s) {
        Bitset res(s.size());
        for (int i = 0; i < s.size(); ++i) {
            char c = s[s.size() - 1 - i];
            if (c == '1') res.modify(i, true);
            else if (c != '0') throw invalid_argument("invalid char in bitstring");
        }
        return res;
    }
};
