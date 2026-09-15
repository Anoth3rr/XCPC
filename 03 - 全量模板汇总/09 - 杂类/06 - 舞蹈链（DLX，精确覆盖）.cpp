/*
用途：求精确覆盖的一组可行行，适合数独、棋盘覆盖等；搜索复杂度取决于实例。
列编号为 1..m，addRow(id, cols) 加入一行；同一行的列不得重复。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
struct DLX {
    int m;
    vector<int> l, r, u, d, row, col, s, cur, ans;

    explicit DLX(int m, int z = 0) : m(m) {
        l.resize(m + 1);
        r.resize(m + 1);
        u.resize(m + 1);
        d.resize(m + 1);
        row.assign(m + 1, 0);
        col.resize(m + 1);
        s.assign(m + 1, 0);
        for (int i = 0; i <= m; ++i) {
            l[i] = i - 1;
            r[i] = i + 1;
            u[i] = d[i] = col[i] = i;
        }
        l[0] = m;
        r[m] = 0;
        if (z) {
            int n = m + 1 + z;
            l.reserve(n), r.reserve(n), u.reserve(n), d.reserve(n);
            row.reserve(n), col.reserve(n);
        }
    }

    void addRow(int id, const vector<int>& a) {
        int h = -1;
        for (int c : a) {
            assert(1 <= c && c <= m);
            int x = l.size();
            l.push_back(x), r.push_back(x), u.push_back(0), d.push_back(0);
            row.push_back(id), col.push_back(c);
            u[x] = u[c], d[x] = c;
            d[u[c]] = x, u[c] = x;
            ++s[c];
            if (h == -1) h = x;
            else {
                l[x] = l[h], r[x] = h;
                r[l[h]] = x, l[h] = x;
            }
        }
    }

    void cover(int c) {
        r[l[c]] = r[c], l[r[c]] = l[c];
        for (int i = d[c]; i != c; i = d[i]) {
            for (int j = r[i]; j != i; j = r[j]) {
                d[u[j]] = d[j], u[d[j]] = u[j];
                --s[col[j]];
            }
        }
    }

    void uncover(int c) {
        for (int i = u[c]; i != c; i = u[i]) {
            for (int j = l[i]; j != i; j = l[j]) {
                ++s[col[j]];
                d[u[j]] = j, u[d[j]] = j;
            }
        }
        r[l[c]] = c, l[r[c]] = c;
    }

    bool dfs() {
        if (!r[0]) {
            ans = cur;
            return true;
        }
        int c = r[0];
        for (int j = r[c]; j; j = r[j]) if (s[j] < s[c]) c = j;
        if (!s[c]) return false;
        cover(c);
        for (int i = d[c]; i != c; i = d[i]) {
            cur.push_back(row[i]);
            for (int j = r[i]; j != i; j = r[j]) cover(col[j]);
            bool f = dfs();
            for (int j = l[i]; j != i; j = l[j]) uncover(col[j]);
            cur.pop_back();
            if (f) {
                uncover(c);
                return true;
            }
        }
        uncover(c);
        return false;
    }

    bool solve() {
        cur.clear();
        ans.clear();
        return dfs();
    }
};
