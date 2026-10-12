struct Trie {
    int n, idx;
    vector<vector<int>> ch;
    Trie(int n) {
        this->n = n;
        idx = 0;
        ch.resize(30 * (n + 1), vector<int>(2));
    }
    void insert(int x) {
        int u = 0;
        for (int i = 30; ~i; i--) {
            int &v = ch[u][x >> i & 1];
            if (!v) v = ++idx;
            u = v;
        }
    }
    int query(int x) {
        int u = 0, res = 0;
        for (int i = 30; ~i; i--) {
            int v = x >> i & 1;
            if (ch[u][!v]) {
                res += (1 << i);
                u = ch[u][!v];
            } else {
                u = ch[u][v];
            }
        }
        return res;
    }
};
