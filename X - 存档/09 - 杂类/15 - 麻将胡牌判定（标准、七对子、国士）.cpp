/*
用途：判断 34 种牌编码下的十四张麻将手牌。
编码：万 0..8、筒 9..17、索 18..26、字牌 27..33；同一张牌最多 4 张。
包含：标准型（四组面子+一对将）、七对子、国士无双；不处理副露、番数和地方规则。
复杂度：标准型回溯状态很小；最坏由牌面种类数决定，实际比赛中可直接使用。
注意：七对子按常见规则允许四张相同牌计作两对；若题面禁止，改 ask7 的条件。
*/
struct Mahjong {
    using C = array<int, 34>;

    static C cnt(const vector<int> &a) {
        C c{};
        for (int x : a) {
            assert(0 <= x && x < 34);
            ++c[x];
            assert(c[x] <= 4);
        }
        return c;
    }

    static bool dfs(C &c) {
        int x = 0;
        while (x < 34 && !c[x]) ++x;
        if (x == 34) return true;
        if (c[x] >= 3) {
            c[x] -= 3;
            if (dfs(c)) return true;
            c[x] += 3;
        }
        if (x < 27 && x % 9 <= 6 && c[x + 1] && c[x + 2]) {
            --c[x], --c[x + 1], --c[x + 2];
            if (dfs(c)) return true;
            ++c[x], ++c[x + 1], ++c[x + 2];
        }
        return false;
    }

    static bool askStd(C c) {
        int s = 0;
        for (int x : c) s += x;
        if (s != 14) return false;
        for (int x = 0; x < 34; ++x) if (c[x] >= 2) {
            c[x] -= 2;
            if (dfs(c)) return true;
            c[x] += 2;
        }
        return false;
    }

    static bool ask7(const C &c) {
        int s = 0, p = 0;
        for (int x : c) {
            s += x;
            if (x & 1) return false;
            p += x / 2;
        }
        return s == 14 && p == 7;
    }

    static bool ask13(const C &c) {
        static constexpr int q[] = {0, 8, 9, 17, 18, 26,
                                     27, 28, 29, 30, 31, 32, 33};
        array<char, 34> ok{};
        for (int x : q) ok[x] = true;
        int s = 0, d = 0;
        for (int x = 0; x < 34; ++x) {
            if (c[x] && !ok[x]) return false;
            s += c[x];
            if (ok[x]) {
                if (!c[x]) return false;
                d += c[x] - 1;
            }
        }
        return s == 14 && d == 1;
    }

    static bool ask(const vector<int> &a) {
        if (a.size() != 14) return false;
        C c = cnt(a);
        return askStd(c) || ask7(c) || ask13(c);
    }
};
