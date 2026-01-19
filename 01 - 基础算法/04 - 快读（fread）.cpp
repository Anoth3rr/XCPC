struct IO {
    char a[1 << 25], b[1 << 25], *s, *t;
    IO() : s(a), t(b) {
        a[fread(a, 1, sizeof a, stdin)] = 0;
    }
    ~IO() {
        fwrite(b, 1, t - b, stdout);
    }
    IO &operator>>(ull &x);
    IO &operator>>(ll &x);
    IO &operator>>(int &x);
    IO &operator>>(unsigned &x) {
        x = 0;

        while (*s < '0' || *s > '9')
            ++s;

        while (*s >= '0' && *s <= '9')
            x = x * 10 + *s++ - '0';

        return *this;
    }
    IO &operator<<(const char *tmp) {
        return fwrite(tmp, 1, strlen(tmp), stdout), *this;
    }
    IO &operator<<(char x) {
        return *t++ = x, *this;
    }
    IO &operator<<(ull x);
    IO &operator<<(ll x);
    IO &operator<<(int x);
    IO &operator<<(unsigned x) {
        static char c[16], *i;
        i = c;

        if (x == 0) {
            *t++ = '0';
        } else {
            while (x != 0) {
                unsigned y = x / 10;
                *i++ = x - y * 10 + '0', x = y;
            }

            while (i != c)
                *t++ = *--i;
        }

        return *this;
    }
} io;