auto kmp = [&](string s, string t) {
    int n = s.size(), m = t.size();
    vector<int> kmp(m + 1), ans;
    s = "@" + s;
    t = "@" + t;
    for (int i = 2, j = 0; i <= m; i++) {
        while (j && t[i] != t[j + 1]) {
            j = kmp[j];
        }
        j += t[i] == t[j + 1];
        kmp[i] = j;
    }
    for (int i = 1, j = 0; i <= n; i++) {
        while (j && s[i] != t[j + 1]) {
            j = kmp[j];
        }
        if (s[i] == t[j + 1] && ++j == m) {
            ans.push_back(i - m + 1); // t 在 s 中出现的位置
        }
    }
    return ans;
};
