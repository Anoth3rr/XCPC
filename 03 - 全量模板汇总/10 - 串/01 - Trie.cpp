struct Trie {
    int ch[N][63], cnt[N], idx = 0;
    map<char, int> mp;
    void init() {
        LL id = 0;
        for (char c = 'a'; c <= 'z'; c++) mp[c] = ++id;
        for (char c = 'A'; c <= 'Z'; c++) mp[c] = ++id;
        for (char c = '0'; c <= '9'; c++) mp[c] = ++id;
    }
    void insert(string s) {
        int u = 0;
        for (int i = 0; i < s.size(); i++) {
            int v = mp[s[i]];
            if (!ch[u][v]) ch[u][v] = ++idx;
            u = ch[u][v];
            cnt[u]++;
        }
    }
    LL query(string s) {
        int u = 0;
        for (int i = 0; i < s.size(); i++) {
            int v = mp[s[i]];
            if (!ch[u][v]) return 0;
            u = ch[u][v];
        }
        return cnt[u];
    }
    void Clear() {
        for (int i = 0; i <= idx; i++) {
            cnt[i] = 0;
            for (int j = 0; j <= 62; j++) {
                ch[i][j] = 0;
            }
        }
        idx = 0;
    }
} trie;
