namespace FastIO {
    char ib[1 << 21], *p1 = ib, *p2 = ib;

    inline int getc() {
        return p1 == p2 && (p2 = (p1 = ib) + fread(ib, 1, 1 << 21, stdin), p1 == p2) ? EOF : *p1++;
    }

    template <typename T> void Cin(T &a) {
        T ans = 0;
        bool f = 0;
        int c = getc();
        for (; c < '0' || c > '9'; c = getc()) {
            if (c == '-') f = 1;
        }
        for (; c >= '0' && c <= '9'; c = getc()) {
            ans = ans * 10 + c - '0';
        }
        a = f ? -ans : ans;
    }

    template <typename T, typename U, typename... A> void Cin(T &a, U &b, A &...c) {
        Cin(a), Cin(b, c...);
    }

    char ob[1 << 21], *p3 = ob, *p4 = ob + (1 << 21);

    inline void flush() {
        fwrite(ob, 1, p3 - ob, stdout);
        p3 = ob;
    }

    inline void putc(char c) {
        if (p3 == p4) flush();
        *p3++ = c;
    }

    template <typename T> void Write(T a) {
        if (a == 0) {
            putc('0');
            return;
        }
        if (a < 0) putc('-'), a = -a;
        char s[40];
        int top = 0;
        while (a) {
            s[top++] = a % 10 + '0';
            a /= 10;
        }
        while (top) putc(s[--top]);
    }

    template <typename T> void Cout(T a) {
        Write(a);
        putc('\n');
    }

    template <typename T, typename U, typename... A> void Cout(T a, U b, A... c) {
        Write(a);
        putc(' ');
        Cout(b, c...);
    }

    struct Flush {
        ~Flush() {
            flush();
        }
    };
    inline Flush io;
} // namespace FastIO

using namespace FastIO;