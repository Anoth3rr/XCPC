template <class Op> struct ST {
    int n;
    vector<vector<int>> st;

    ST(const vector<int> &a) : n(a.size() - 1) {
        st.push_back(a);
        for (int i = 1; (1 << i) <= n; ++i) {
            int l = 1 << i, h = l >> 1;
            st.emplace_back(n - l + 2);
            for (int j = 1; j + l - 1 <= n; ++j) {
                st[i][j] = Op{}(st[i - 1][j], st[i - 1][j + h]);
            }
        }
    }
    int ask(int l, int r) const {
        int k = __lg(r - l + 1);
        return Op{}(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
