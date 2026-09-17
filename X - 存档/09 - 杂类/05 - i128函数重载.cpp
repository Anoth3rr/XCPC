using i128 = __int128_t;
using u128 = __uint128_t;

inline bool read128(const std::string &s, i128 &n) {
    if (s.empty()) return false;

    int pos = 0;
    bool y = false;
    if (s[pos] == '-' || s[pos] == '+') {
        y = (s[pos] == '-');
        ++pos;
    }
    if (pos == s.size()) return false;

    const u128 lim = y ? (u128{1} << 127) : ((u128{1} << 127) - 1);
    u128 val = 0;
    for (; pos < s.size(); ++pos) {
        const char c = s[pos];
        if (c < '0' || c > '9') return false;
        const u128 dig = static_cast<u128>(c - '0');
        if (val > (lim - dig) / 10) return false;
        val = val * 10 + dig;
    }

    if (y) {
        n = (val == lim) ? -static_cast<i128>(lim - 1) - 1 : -static_cast<i128>(val);
    } else {
        n = static_cast<i128>(val);
    }
    return true;
}

inline std::istream &operator>>(std::istream &is, i128 &n) {
    std::string s;
    if (!(is >> s)) return is;
    if (!read128(s, n)) is.setstate(std::ios::failbit);
    return is;
}

inline std::ostream &operator<<(std::ostream &os, i128 n) {
    if (n == 0) return os << '0';

    u128 val;
    if (n < 0) {
        os << '-';
        val = u128{0} - static_cast<u128>(n);
    } else {
        val = static_cast<u128>(n);
    }

    std::string s;
    while (val > 0) {
        s += static_cast<char>('0' + val % 10);
        val /= 10;
    }
    std::reverse(s.begin(), s.end());
    return os << s;
}

inline i128 toi128(const std::string &s) {
    i128 n = 0;
    assert(read128(s, n));
    return n;
}
