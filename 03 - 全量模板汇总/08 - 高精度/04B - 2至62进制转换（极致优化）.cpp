/*
用途：超长 2..62 进制整数转换；依赖已选择的 01A 或 01B `BigInt`。
优化：按不超过 1e9 的大块读入/除出，显著减少高精度乘除次数。
字符顺序：0-9A-Za-z。
*/
struct BaseConv {
    static int val(char c) {
        if (isdigit((unsigned char)c)) return c - '0';
        if (isupper((unsigned char)c)) return c - 'A' + 10;
        return c - 'a' + 36;
    }
    static char chr(int x) {
        if (x < 10) return char('0' + x);
        if (x < 36) return char('A' + x - 10);
        return char('a' + x - 36);
    }
    static pii block(int b) {
        int p = 1, k = 0;
        while (p <= 1000000000 / b) p *= b, ++k;
        return {p, k};
    }

    static string cvt(string s, int a, int b) {
        assert(2 <= a && a <= 62 && 2 <= b && b <= 62);
        bool neg = !s.empty() && s[0] == '-';
        if (neg) s.erase(s.begin());
        int p = 0;
        while (p < s.size() && s[p] == '0') ++p;
        s.erase(0, p);
        if (s.empty()) return "0";

        auto [pa, ka] = block(a);
        BigInt x = 0;
        int f = s.size() % ka;
        if (!f) f = ka;
        for (int l = 0; l < s.size();) {
            int z = l ? ka : f, q = 1, v = 0;
            for (int i = 0; i < z; ++i) {
                int d = val(s[l + i]);
                if (d >= a) throw invalid_argument("FastBaseConv digit");
                v = v * a + d;
                q *= a;
            }
            x = x * BigInt(q) + BigInt(v);
            l += z;
        }

        auto [pb, kb] = block(b);
        string z;
        while (!x.ask0()) {
            auto [q, r] = BigInt::divmod(x, pb);
            x = move(q);
            for (int i = 0; i < kb; ++i) {
                z += chr(r % b);
                r /= b;
            }
        }
        while (z.size() > 1 && z.back() == '0') z.pop_back();
        if (neg) z += '-';
        reverse(z.begin(), z.end());
        return z;
    }
};
