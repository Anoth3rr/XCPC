class Bitset {
public:
    using ull = uint64_t;
    static constexpr int WORD = 64;

private:
    vector<ull> data_;
    int nbits_;
    ull last_mask_;

    static constexpr int words_for(int bits) {
        return (bits + WORD - 1) / WORD;
    }

    void update_last_mask() {
        int r = nbits_ % WORD;
        if (r == 0) last_mask_ = ~ull(0);
        else last_mask_ = (1ull << r) - 1;
        if (!data_.empty()) data_.back() &= last_mask_;
    }

public:
    Bitset() : nbits_(0), last_mask_(~ull(0)) {}
    explicit Bitset(int n, bool val = false) : data_(words_for(n), val ? ~ull(0) : 0), nbits_(n) {
        update_last_mask();
        if (val && (nbits_ % WORD)) data_.back() &= last_mask_;
    }

    int size() const noexcept { return nbits_; }
    bool empty() const noexcept { return nbits_ == 0; }
    void reset() noexcept { fill(data_.begin(), data_.end(), 0); }
    void set() noexcept { fill(data_.begin(), data_.end(), ~ull(0)); if (!data_.empty()) data_.back() &= last_mask_; }

    void resize(int n, bool val = false) {
        int old_words = data_.size();
        nbits_ = n;
        int new_words = words_for(n);
        data_.resize(new_words, val ? ~ull(0) : 0);
        update_last_mask();
        if (val && new_words > 0 && (nbits_ % WORD)) data_.back() &= last_mask_;
    }

    void set(int pos) {
        if (pos >= nbits_) throw out_of_range("bit index out of range");
        data_[pos / WORD] |= (1ull << (pos % WORD));
    }
    void reset(int pos) {
        if (pos >= nbits_) throw out_of_range("bit index out of range");
        data_[pos / WORD] &= ~(1ull << (pos % WORD));
    }
    void flip(int pos) {
        if (pos >= nbits_) throw out_of_range("bit index out of range");
        data_[pos / WORD] ^= (1ull << (pos % WORD));
    }
    bool test(int pos) const {
        if (pos >= nbits_) throw out_of_range("bit index out of range");
        return (data_[pos / WORD] >> (pos % WORD)) & 1;
    }

    bool any() const noexcept {
        for (auto x : data_) if (x) return true;
        return false;
    }
    bool none() const noexcept { return !any(); }

    int count() const noexcept {
        int s = 0;
        for (auto x : data_) s += (int)__builtin_popcountll(x);
        return s;
    }

    Bitset& operator&=(const Bitset& o) {
        assert(nbits_ == o.nbits_);
        int m = data_.size();
        for (int i = 0; i < m; ++i) data_[i] &= o.data_[i];
        return *this;
    }
    Bitset& operator|=(const Bitset& o) {
        assert(nbits_ == o.nbits_);
        int m = data_.size();
        for (int i = 0; i < m; ++i) data_[i] |= o.data_[i];
        return *this;
    }
    Bitset& operator^=(const Bitset& o) {
        assert(nbits_ == o.nbits_);
        int m = data_.size();
        for (int i = 0; i < m; ++i) data_[i] ^= o.data_[i];
        if (!data_.empty()) data_.back() &= last_mask_;
        return *this;
    }
    Bitset operator~() const {
        Bitset r(*this);
        for (auto &x : r.data_) x = ~x;
        if (!r.data_.empty()) r.data_.back() &= r.last_mask_;
        return r;
    }

    friend Bitset operator&(Bitset a, const Bitset& b) { a &= b; return a; }
    friend Bitset operator|(Bitset a, const Bitset& b) { a |= b; return a; }
    friend Bitset operator^(Bitset a, const Bitset& b) { a ^= b; return a; }

    Bitset& operator<<=(int shift) {
        if (shift == 0 || nbits_ == 0) return *this;
        int word_shift = shift / WORD;
        int bit_shift = shift % WORD;
        int m = data_.size();
        if (word_shift >= m) {
            reset();
            return *this;
        }
        if (bit_shift == 0) {
            for (int i = m; i-- > word_shift; ) data_[i] = data_[i - word_shift];
        } else {
            for (int i = m; i-- > word_shift; ) {
                ull high = data_[i - word_shift] << bit_shift;
                ull low = 0;
                if (i - word_shift > 0) low = data_[i - word_shift - 1] >> (WORD - bit_shift);
                data_[i] = high | low;
            }
        }
        for (int i = 0; i < word_shift; ++i) data_[i] = 0;
        if (!data_.empty()) data_.back() &= last_mask_;
        return *this;
    }

    Bitset& operator>>=(int shift) {
        if (shift == 0 || nbits_ == 0) return *this;
        int word_shift = shift / WORD;
        int bit_shift = shift % WORD;
        int m = data_.size();
        if (word_shift >= m) {
            reset();
            return *this;
        }
        if (bit_shift == 0) {
            for (int i = 0; i + word_shift < m; ++i) data_[i] = data_[i + word_shift];
        } else {
            for (int i = 0; i + word_shift < m; ++i) {
                ull low = data_[i + word_shift] >> bit_shift;
                ull high = 0;
                if (i + word_shift + 1 < m) high = data_[i + word_shift + 1] << (WORD - bit_shift);
                data_[i] = low | high;
            }
        }
        for (int i = m - word_shift; i < m; ++i) data_[i] = 0;
        if (!data_.empty()) data_.back() &= last_mask_;
        return *this;
    }

    friend Bitset operator<<(Bitset a, int s) { a <<= s; return a; }
    friend Bitset operator>>(Bitset a, int s) { a >>= s; return a; }

    int next_set(int pos) const noexcept {
        if (pos >= nbits_) return nbits_;
        int idx = pos / WORD;
        unsigned offset = pos % WORD;
        ull w = data_[idx] & (~ull(0) << offset);
        if (w) return idx * WORD + __builtin_ctzll(w);
        ++idx;
        while (idx < data_.size()) {
            if (data_[idx]) return idx * WORD + __builtin_ctzll(data_[idx]);
            ++idx;
        }
        return nbits_;
    }

    int prev_set(int pos) const noexcept {
        if (nbits_ == 0) return nbits_;
        if (pos >= nbits_) pos = nbits_ - 1;
        int idx = pos / WORD;
        unsigned offset = pos % WORD;
        ull w = data_[idx] & ((offset == 63) ? ~ull(0) : ((1ull << (offset + 1)) - 1));
        if (w) return idx * WORD + (63 - __builtin_clzll(w));
        if (idx == 0) return nbits_;
        do {
            --idx;
            if (data_[idx]) return idx * WORD + (63 - __builtin_clzll(data_[idx]));
        } while (idx > 0);
        return nbits_;
    }

    template<typename F>
    void for_each_set(F f) const {
        int base = 0;
        for (int i = 0; i < data_.size(); ++i) {
            ull w = data_[i];
            while (w) {
                unsigned t = __builtin_ctzll(w);
                f(base + t);
                w &= w - 1;
            }
            base += WORD;
        }
    }

    string to_string() const {
        string s;
        s.reserve(nbits_);
        for (int i = 0; i < nbits_; ++i) s.push_back(test(nbits_ - 1 - i) ? '1' : '0');
        return s;
    }

    const ull* data() const noexcept { return data_.empty() ? nullptr : data_.data(); }
    ull* data() noexcept { return data_.empty() ? nullptr : data_.data(); }

    static Bitset from_string(const string& s) {
        Bitset bs(s.size());
        for (int i = 0; i < s.size(); ++i) {
            char c = s[s.size() - 1 - i];
            if (c == '1') bs.set(i);
            else if (c != '0') throw invalid_argument("invalid char in bitstring");
        }
        return bs;
    }
};