/*
用途：完全二分图的最小/最大权完备匹配（Hungarian / KM）。
约定：左部大小 <= 右部大小；每个左部点都必须匹配一个不同右部点，需为每一对点设置权值。
askMin/askMax 分别返回最小/最大权值；权值及答案应远离 int 边界。
复杂度：O(leftSize^2 * rightSize)。
*/

struct KM {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    struct Result {
        int val;
        vector<int> ml;   // matchLeft[u] = 匹配到的右部点。
        vector<int> mr;  // matchRight[v] = 匹配到的左部点，0 表示未使用。
    };

    int nl, nr;
    vector<vector<int>> cst;

    KM(int nl, int nr)
        : nl(nl), nr(nr), cst(nl + 1, vector<int>(nr + 1)) {
        assert(nl <= nr);
    }

    void modify(int l, int r, int v) {
        assert(1 <= l && l <= nl && 1 <= r && r <= nr);
        cst[l][r] = v;
    }

    Result askMin() const {
        vector<int> hl(nl + 1), hr(nr + 1);
        vector<int> mr(nr + 1), pre(nr + 1);

        for (int x = 1; x <= nl; ++x) {
            mr[0] = x;
            int r0 = 0;
            vector<int> mn(nr + 1, INF);
            vector<char> use(nr + 1, false);

            do {
                use[r0] = true;
                int l = mr[r0];
                int d = INF;
                int r = 0;
                for (int y = 1; y <= nr; ++y) {
                    if (use[y]) continue;
                    int rc = cst[l][y] - hl[l] - hr[y];
                    if (rc < mn[y]) {
                        mn[y] = rc;
                        pre[y] = r0;
                    }
                    if (mn[y] < d) {
                        d = mn[y];
                        r = y;
                    }
                }
                for (int y = 0; y <= nr; ++y) {
                    if (use[y]) {
                        hl[mr[y]] += d;
                        hr[y] -= d;
                    } else {
                        mn[y] -= d;
                    }
                }
                r0 = r;
            } while (mr[r0] != 0);

            do {
                int prv = pre[r0];
                mr[r0] = mr[prv];
                r0 = prv;
            } while (r0 != 0);
        }

        vector<int> ml(nl + 1);
        for (int y = 1; y <= nr; ++y) {
            if (mr[y] != 0) ml[mr[y]] = y;
        }
        return {-hr[0], move(ml), move(mr)};
    }

    Result askMax() const {
        KM neg(nl, nr);
        for (int x = 1; x <= nl; ++x) {
            for (int y = 1; y <= nr; ++y) {
                neg.cst[x][y] = -cst[x][y];
            }
        }
        Result res = neg.askMin();
        res.val = -res.val;
        return res;
    }
};
