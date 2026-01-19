struct KMP {
    string pat;
    vector<int> nxt;

    KMP(const string &p) : pat(p) {
        build();
    }

    void build() {
        int m = pat.size();
        nxt.assign(m, 0);

        for (int i = 1, j = 0; i < m; i++) {
            while (j > 0 && pat[i] != pat[j]) {
                j = nxt[j - 1];
            }
            if (pat[i] == pat[j]) {
                j++;
            }
            nxt[i] = j;
        }
    }

    vector<int> match(const string &text) {
        vector<int> res;
        int n = text.size(), m = pat.size();

        for (int i = 0, j = 0; i < n; i++) {
            while (j > 0 && text[i] != pat[j]) {
                j = nxt[j - 1];
            }
            if (text[i] == pat[j]) {
                j++;
            }
            if (j == m) {
                res.push_back(i - m + 1);
                j = nxt[j - 1];
            }
        }
        return res;
    }
};