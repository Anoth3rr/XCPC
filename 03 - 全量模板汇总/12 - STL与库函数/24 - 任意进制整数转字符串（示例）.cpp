/**
    @brief 将整数 x 转换为 base 进制的字符串
    @param x 待转换的整数：[-2^63, 2^63-1]
    @param base 进制：[2, 36]
    @return 转换后的字符串
*/
string my_itoa(i64 x, int base) {
    static const string dig = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (base < 2 || base > 36) return {};
    if (x == 0) return "0";

    bool f = x < 0;
    string s;
    while (x != 0) {
        int d = x % base;
        if (d < 0) d = -d;
        s.push_back(dig[d]);
        x /= base;
    }
    if (f) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}
