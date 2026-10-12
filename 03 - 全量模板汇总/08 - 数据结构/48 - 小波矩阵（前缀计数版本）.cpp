struct WaveletMatrix {
    int lg;
    vector<i64> val;
    vector<int> mid;
    vector<vector<int>> z; // 每层的 0 前缀和

    WaveletMatrix(const vector<i64> &a) {
        int n = a.size() - 1;
        val.assign(a.begin() + 1, a.end());
        sort(val.begin(), val.end());
        val.erase(unique(val.begin(), val.end()), val.end());

        int m = val.size();
        lg = m == 1 ? 1 : __lg(m - 1) + 1;

        vector<int> cur(n + 1), nxt(n + 1);
        for (int i = 1; i <= n; i++) {
            cur[i] = lower_bound(val.begin(), val.end(), a[i]) - val.begin();
        }

        mid.resize(lg);
        z.assign(lg, vector<int>(n + 1));
        for (int d = 0; d < lg; d++) {
            int pos = lg - 1 - d;
            for (int i = 1; i <= n; i++) {
                z[d][i] = z[d][i - 1] + ((cur[i] >> pos & 1) == 0);
            }

            mid[d] = z[d][n];
            int p0 = 0, p1 = mid[d];
            for (int i = 1; i <= n; i++) {
                if (cur[i] >> pos & 1) nxt[++p1] = cur[i];
                else nxt[++p0] = cur[i];
            }
            cur.swap(nxt);
        }
    }

    i64 kth(int L, int R, int k) const {
        int ans = 0;
        for (int d = 0; d < lg; d++) {
            int zL = z[d][L - 1], zR = z[d][R];
            if (k <= zR - zL) {
                L = zL + 1, R = zR;
            } else {
                k -= zR - zL, ans |= 1 << (lg - 1 - d);
                L = mid[d] + L - zL, R = mid[d] + R - zR;
            }
        }
        return val[ans];
    }

    i64 kthLarge(int L, int R, int k) const {
        return kth(L, R, R - L + 2 - k);
    }
};
