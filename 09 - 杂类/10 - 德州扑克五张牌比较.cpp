/*
用途：比较两手五张德州扑克；返回负数/0/正数。
输入牌用两字符表示，如 AS、TD；第二字符花色只需相同可比较。
牌型从高到低：同花顺、四条、葫芦、同花、顺子、三条、两对、一对、高牌。
*/
struct Poker5 {
    static int rk(char c) {
        string s = "--23456789TJQKA";
        return s.find(c);
    }

    static vector<int> val(const array<string, 5> &a) {
        array<int, 15> c{};
        vector<int> r;
        bool f = true;
        for (auto &s : a) {
            r.push_back(rk(s[0]));
            ++c[r.back()];
            f &= s[1] == a[0][1];
        }
        sort(r.begin(), r.end(), greater<>());
        vector<int> q = r;
        q.erase(unique(q.begin(), q.end()), q.end());
        int h = 0;
        if (q.size() == 5 && q[0] - q[4] == 4) h = q[0];
        if (q == vector<int>({14, 5, 4, 3, 2})) h = 5;

        vector<pii> g;
        for (int x = 2; x <= 14; ++x) if (c[x]) g.push_back({c[x], x});
        sort(g.begin(), g.end(), greater<>());
        if (f && h) return {8, h};
        if (g[0][0] == 4) return {7, g[0][1], g[1][1]};
        if (g[0][0] == 3 && g[1][0] == 2) return {6, g[0][1], g[1][1]};
        if (f) return {5, r[0], r[1], r[2], r[3], r[4]};
        if (h) return {4, h};
        if (g[0][0] == 3) return {3, g[0][1], g[1][1], g[2][1]};
        if (g[0][0] == 2 && g[1][0] == 2) return {2, g[0][1], g[1][1], g[2][1]};
        if (g[0][0] == 2) return {1, g[0][1], g[1][1], g[2][1], g[3][1]};
        return {0, r[0], r[1], r[2], r[3], r[4]};
    }

    static int cmp(const array<string, 5> &a, const array<string, 5> &b) {
        auto x = val(a), y = val(b);
        return (x > y) - (x < y);
    }
};
