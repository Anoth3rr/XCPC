/*
用途：二进制 Trie（XOR Trie），维护可重集合并查询异或最值。

U 必须为无符号整数类型，B 默认为其完整位数。
接口：insert(x)、erase(x)、askMax(x)、askMin(x)、ask(x,lim)。
askMax/askMin 在空 Trie 时返回 nullopt；ask 统计所有 y 使 (x xor y) < lim。
*/

using u64 = uint64_t;

template <class U = u64, int B = numeric_limits<U>::digits>
struct XorTrie {
    static_assert(is_unsigned_v<U>);

    struct Node {
        array<int, 2> nxt{};
        int cnt = 0;
    };

    vector<Node> tr = {Node{}};

    int askSz() const {
        return tr[0].cnt;
    }

    bool askEmp() const {
        return askSz() == 0;
    }

    void insert(U val) {
        int u = 0;
        ++tr[u].cnt;
        for (int bit = B - 1; bit >= 0; --bit) {
            int b = (val >> bit) & U(1);
            if (!tr[u].nxt[b]) {
                tr[u].nxt[b] = tr.size();
                tr.emplace_back();
            }
            u = tr[u].nxt[b];
            ++tr[u].cnt;
        }
    }

    bool erase(U val) {
        int u = 0;
        vector<int> pth = {0};
        for (int bit = B - 1; bit >= 0; --bit) {
            int b = (val >> bit) & U(1);
            if (!tr[u].nxt[b] || tr[tr[u].nxt[b]].cnt == 0) return false;
            u = tr[u].nxt[b];
            pth.push_back(u);
        }
        for (int p : pth) --tr[p].cnt;
        return true;
    }

    optional<U> askMax(U val) const {
        if (askEmp()) return nullopt;
        U ans = 0;
        int u = 0;
        for (int bit = B - 1; bit >= 0; --bit) {
            int b = (val >> bit) & U(1);
            int pre = b ^ 1;
            if (tr[u].nxt[pre] && tr[tr[u].nxt[pre]].cnt) {
                ans |= U(1) << bit;
                u = tr[u].nxt[pre];
            } else {
                u = tr[u].nxt[b];
            }
        }
        return ans;
    }

    optional<U> askMin(U val) const {
        if (askEmp()) return nullopt;
        U ans = 0;
        int u = 0;
        for (int bit = B - 1; bit >= 0; --bit) {
            int b = (val >> bit) & U(1);
            if (tr[u].nxt[b] && tr[tr[u].nxt[b]].cnt) {
                u = tr[u].nxt[b];
            } else {
                ans |= U(1) << bit;
                u = tr[u].nxt[b ^ 1];
            }
        }
        return ans;
    }

    int ask(U val, U lim) const {
        int u = 0;
        int ans = 0;
        for (int bit = B - 1; bit >= 0; --bit) {
            int xb = (val >> bit) & U(1);
            int b = (lim >> bit) & U(1);
            if (b) {
                int sam = tr[u].nxt[xb];
                if (sam) ans += tr[sam].cnt;
                u = tr[u].nxt[xb ^ 1];
            } else {
                u = tr[u].nxt[xb];
            }
            if (!u) break;
        }
        return ans;
    }
};
