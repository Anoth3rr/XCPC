namespace FastIO {
    constexpr int BSZ = 1 << 21;

    char ib[BSZ];
    int p1 = 0, p2 = 0;

    inline int getc() {
        if (p1 >= p2) {
            p2 = fread(ib, 1, BSZ, stdin);
            p1 = 0;
            if (p2 == 0) return EOF;
        }
        return static_cast<unsigned char>(ib[p1++]);
    }

    template <typename T> bool Cin(T &a) {
        static_assert(std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>);

        int c = getc();
        while (c != EOF && (c < '0' || c > '9') && c != '-' && c != '+') c = getc();
        if (c == EOF) return false;

        bool y = false;
        if (c == '-') {
            if constexpr (!std::is_signed_v<T>) return false;
            y = true;
            c = getc();
        } else if (c == '+') {
            c = getc();
        }
        if (c < '0' || c > '9') return false;

        using U = std::make_unsigned_t<T>;
        const U lim = [&] {
            if constexpr (std::is_signed_v<T>) {
                return y ? static_cast<U>(std::numeric_limits<T>::max()) + U{1} : static_cast<U>(std::numeric_limits<T>::max());
            } else {
                return std::numeric_limits<T>::max();
            }
        }();

        U val = 0;
        while (c >= '0' && c <= '9') {
            const U dig = c - '0';
            if (val > (lim - dig) / 10) return false;
            val = val * 10 + dig;
            c = getc();
        }

        if constexpr (std::is_signed_v<T>) {
            if (y) {
                a = (val == lim) ? std::numeric_limits<T>::min() : -static_cast<T>(val);
            } else {
                a = static_cast<T>(val);
            }
        } else {
            a = val;
        }
        return true;
    }

    template <typename T, typename U, typename... A> bool Cin(T &a, U &b, A &...c) {
        return Cin(a) && Cin(b, c...);
    }

    char ob[BSZ];
    int p3 = 0;

    inline void flush() {
        if (p3 == 0) return;
        fwrite(ob, 1, p3, stdout);
        p3 = 0;
    }

    inline void putc(char c) {
        if (p3 == BSZ) flush();
        ob[p3++] = c;
    }

    template <typename T> void Cout(T x, char end = '\n') {
        static_assert(std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>);
        using U = std::make_unsigned_t<T>;

        U val;
        if constexpr (std::is_signed_v<T>) {
            if (x < 0) {
                putc('-');
                val = U{0} - static_cast<U>(x);
            } else {
                val = static_cast<U>(x);
            }
        } else {
            val = x;
        }

        char s[std::numeric_limits<U>::digits10 + 3];
        int n = 0;
        do {
            s[n++] = static_cast<char>('0' + val % 10);
            val /= 10;
        } while (val > 0);
        while (n > 0) putc(s[--n]);
        putc(end);
    }

    struct Flush {
        ~Flush() {
            flush();
        }
    };
    inline Flush io;
} // namespace FastIO

using namespace FastIO;
