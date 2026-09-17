/*
用途：Linux mmap + 分块表的极致整数输入输出。
限制：仅 Linux/POSIX；Windows 或需要可移植性的题目使用 04 - fastio。
性能：普通文件输入走 mmap，管道输入回退 read；输出为连续缓冲，常数显著小于 iostream；不支持交互题。
*/
#ifdef __linux__
    #include <fcntl.h>
    #include <cerrno>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

#ifdef __linux__
namespace fastio {
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;
    using u8 = std::uint8_t;

    struct Tables {
        alignas(64) std::int8_t r2[1 << 16];
        alignas(64) u32 p4[10000];

        Tables() {
            std::memset(r2, -1, sizeof(r2));

            for (int hi = '0'; hi <= '9'; ++hi)
                for (int lo = '0'; lo <= '9'; ++lo) r2[(hi << 8) | lo] = (lo & 15) * 10 + (hi & 15);

            for (u32 x = 0; x < 10000; ++x) {
                p4[x] = u32('0' + x / 1000) | (u32('0' + x / 100 % 10) << 8) | (u32('0' + x / 10 % 10) << 16) | (u32('0' + x % 10) << 24);
            }
        }
    };

    static Tables tab;

    struct Input {
        std::vector<char> buf;
        const char *p = nullptr;
        const char *e = nullptr;
        alignas(64) char tail[64]{};
        bool mm = false;

        Input() {
            struct stat st{};
            if (fstat(STDIN_FILENO, &st) == 0 && S_ISREG(st.st_mode) && st.st_size > 0) {
                void *q = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
                if (q != MAP_FAILED) {
                    madvise(q, st.st_size, MADV_SEQUENTIAL);
                    p = reinterpret_cast<const char *>(q);
                    e = p + st.st_size;
                    mm = true;
                    return;
                }
            }

            // 评测机常把 stdin 接成管道；此时 mmap 不可用，回退到一次性 read。
            buf.reserve(1u << 20);
            char t[1u << 16];
            for (;;) {
                ssize_t n = read(STDIN_FILENO, t, sizeof(t));
                if (n > 0) {
                    buf.insert(buf.end(), t, t + n);
                    continue;
                }
                if (n < 0 && errno == EINTR) continue;
                break;
            }
            buf.push_back('\n');
            buf.push_back('\0');
            p = buf.data();
            e = p + buf.size();
        }

        void guard(const char *&q) {
            if (!mm || static_cast<std::size_t>(e - q) >= 32) return;
            std::size_t n = e - q;
            std::memcpy(tail, q, n);
            std::memset(tail + n, 0, sizeof(tail) - n);
            q = tail;
            mm = false;
        }
    };

    static Input in;
    static const char *cur = in.p;

#define READ2(N, LABEL)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
    do {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
        const int d = tab.r2[*reinterpret_cast<const std::uint16_t *>(cur)];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           \
        if (__builtin_expect(d < 0, 0)) goto LABEL;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
        (N) = (N) * 100u + static_cast<unsigned>(d);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   \
        cur += 2;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
    } while (0)

#define IS_DIGIT(C) (static_cast<unsigned>(static_cast<unsigned char>(C) - '0') < 10u)

#define LAST_AND_NEXT(N)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
    do {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
        if (IS_DIGIT(*cur)) (N) = (N) * 10u + static_cast<unsigned>(*cur++ & 15);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
        while (__builtin_expect(static_cast<unsigned>(static_cast<unsigned char>(*++cur)) - 1u < 32u, 0)) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
        }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              \
        return (N);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    } while (0)

    static inline u32 ru32_body() {
        u32 n = 0;

        READ2(n, ru32_done);
        READ2(n, ru32_done);
        READ2(n, ru32_done);
        READ2(n, ru32_done);
        READ2(n, ru32_done);

    ru32_done:
        LAST_AND_NEXT(n);
    }

    static inline u64 ru64_body() {
        u64 n = 0;
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);
        READ2(n, ru64_done);

    ru64_done:
        LAST_AND_NEXT(n);
    }

#undef READ2
#undef IS_DIGIT
#undef LAST_AND_NEXT

    static inline u32 ru32() {
        in.guard(cur);
        if (__builtin_expect(*cur == '+', 0)) ++cur;
        return ru32_body();
    }

    static inline u64 ru64() {
        in.guard(cur);
        if (__builtin_expect(*cur == '+', 0)) ++cur;
        return ru64_body();
    }

    static inline i32 r32() {
        in.guard(cur);
        const bool neg = (*cur == '-');
        cur += neg;
        const u32 x = ru32_body();

        return static_cast<i32>(neg ? 0u - x : x);
    }

    static inline i64 r64() {
        in.guard(cur);
        const bool neg = (*cur == '-');
        cur += neg;
        const u64 x = ru64_body();

        return static_cast<i64>(neg ? 0ull - x : x);
    }

    static constexpr std::size_t OUT_BYTES = 1u << 22;
    alignas(64) static u8 obuf[OUT_BYTES + 32];
    static u8 *now = obuf;

    static inline void flush() {
        if (now != obuf) {
            std::fwrite(obuf, 1, now - obuf, stdout);
            now = obuf;
        }
    }

    static inline void reserve(std::size_t n) {
        if (__builtin_expect(now + n > obuf + OUT_BYTES, 0)) flush();
    }

    template <class U, int K> static inline void pu_unchecked(U x) {
        u8 *const end = now + K;
        u8 *p = end;

        while (x > 9999) {
            p -= 4;
            *reinterpret_cast<u32 *>(p) = tab.p4[static_cast<u32>(x % 10000)];
            x /= 10000;
        }

        const u32 w = tab.p4[static_cast<u32>(x)];

        if (x < 10) {
            *--p = static_cast<u8>('0' + x);
        } else if (x < 100) {
            p -= 2;
            *reinterpret_cast<std::uint16_t *>(p) = static_cast<std::uint16_t>(w >> 16);
        } else if (x < 1000) {
            p -= 4;
            *reinterpret_cast<u32 *>(p) = w;
            ++p;
        } else {
            p -= 4;
            *reinterpret_cast<u32 *>(p) = w;
        }

        std::memmove(now, p, end - p);
        now += end - p;
    }

    static inline void p32(u32 x) {
        reserve(10);
        pu_unchecked<u32, 10>(x);
    }

    static inline void p64(u64 x) {
        reserve(20);
        pu_unchecked<u64, 20>(x);
    }

    static inline void p32(i32 x) {
        reserve(11);
        if (x < 0) {
            *now++ = '-';
            pu_unchecked<u32, 10>(0u - static_cast<u32>(x));
        } else {
            pu_unchecked<u32, 10>(static_cast<u32>(x));
        }
    }

    static inline void p64(i64 x) {
        reserve(21);
        if (x < 0) {
            *now++ = '-';
            pu_unchecked<u64, 20>(0ull - static_cast<u64>(x));
        } else {
            pu_unchecked<u64, 20>(static_cast<u64>(x));
        }
    }

    static inline void pc(char c) {
        reserve(1);
        *now++ = static_cast<u8>(c);
    }

    template <class T> static inline void write(T x) {
        if constexpr (sizeof(T) <= 4) {
            if constexpr (std::is_signed_v<T>)
                p32(static_cast<i32>(x));
            else
                p32(static_cast<u32>(x));
        } else {
            if constexpr (std::is_signed_v<T>)
                p64(static_cast<i64>(x));
            else
                p64(static_cast<u64>(x));
        }
    }

    struct Flush {
        ~Flush() { flush(); }
    };
    static Flush out;

} // namespace fastio
#endif
