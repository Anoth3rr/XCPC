namespace my128 {
    using i128 = int128_t;
    i128 abs(const i128 &x) {
        return x > 0 ? x : -x;
    }
    auto &operator>>(istream &it, i128 &j) {
        string val;
        it >> val;
        reverse(val.begin(), val.end());
        i128 ans = 0;
        bool f = 0;
        char c = val.back();
        val.pop_back();
        for (; c < '0' || c > '9'; c = val.back(), val.pop_back()) {
            if (c == '-') {
                f = 1;
            }
        }
        for (; c >= '0' && c <= '9'; c = val.back(), val.pop_back()) {
            ans = ans * 10 + c - '0';
        }
        j = f ? -ans : ans;
        return it;
    }
    auto &operator<<(ostream &os, const i128 &j) {
        string ans;
        auto write = [&](auto &&self, i128 x) {
            if (x < 0) ans += '-', x = -x;
            if (x > 9) self(self, x / 10);
            ans += x % 10 + '0';
        };
        write(write, j);
        return os << ans;
    }
} // namespace my128
using namespace my128;