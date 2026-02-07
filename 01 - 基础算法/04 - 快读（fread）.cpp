namespace FastIO {
static const int BUFSIZE = 1 << 20;
char IBUF[BUFSIZE];
int p1 = 0, p2 = 0;
inline char getc() {
    if (p1 >= p2) {
        p2 = fread(IBUF, 1, BUFSIZE, stdin);
        p1 = 0;
        if (p2 == 0)
            return EOF;
    }
    return IBUF[p1++];
}
template <typename T>
void Cin(T &a) {
    T ans = 0;
    bool f = 0;
    char c = getc();
    for (; c < '0' || c > '9'; c = getc()) {
        if (c == '-')
            f = 1;
    }
    for (; c >= '0' && c <= '9'; c = getc()) {
        ans = ans * 10 + c - '0';
    }
    a = f ? -ans : ans;
}
template <typename T, typename... Args>
void Cin(T &a, Args &...args) {
    Cin(a), Cin(args...);
}
char OBUF[BUFSIZE];
int p3 = 0;
inline void flush() {
    fwrite(OBUF, 1, p3, stdout);
    p3 = 0;
}
inline void putc(char c) {
    if (p3 >= BUFSIZE)
        flush();
    OBUF[p3++] = c;
}
template <typename T>
void Cout(T x, char end = '\n') {
    if (x == 0) {
        putc('0');
        putc(end);
        return;
    }

    if (x < 0) {
        putc('-');
        x = -x;
    }

    char s[24];
    int n = 0;
    while (x) {
        s[n++] = char('0' + x % 10);
        x /= 10;
    }
    while (n--)
        putc(s[n]);
    putc(end);
}
} // namespace FastIO

using namespace FastIO;