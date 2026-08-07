/*
用途：非负/负整数字符串在 2..62 进制间精确转换，不依赖 BigInt。
复杂度：逐次短除，O(输入位数*输出位数)；普通长度优先使用。
字符顺序：0-9A-Za-z。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
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

    static string cvt(string s, int a, int b) {
        assert(2 <= a && a <= 62 && 2 <= b && b <= 62);
        bool neg = !s.empty() && s[0] == '-';
        if (neg) s.erase(s.begin());
        vector<int> d;
        for (char c : s) {
            int x = val(c);
            if (x >= a) throw invalid_argument("BaseConv digit");
            if (!d.empty() || x) d.push_back(x);
        }
        if (d.empty()) return "0";
        string z;
        while (!d.empty()) {
            int r = 0;
            for (auto &x : d) {
                int y = r * a + x;
                x = y / b;
                r = y % b;
            }
            z += chr(r);
            auto p = find_if(d.begin(), d.end(), [](int x) { return x != 0; });
            d.erase(d.begin(), p);
        }
        if (neg) z += '-';
        reverse(z.begin(), z.end());
        return z;
    }
};
