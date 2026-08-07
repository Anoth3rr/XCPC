/*
用途：定长连续字符集 Trie，支持插入、删除、单词/前缀计数与最长前缀词链。
用法：Trie<26, 'a'>；节点转移、编号和计数使用 int。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <int A = 26, char F = 'a'> struct Trie {

    struct Node {
        array<int, A> to{};
        int ps = 0, ed = 0;
    };

    vector<Node> tr;

    Trie() {
        clear();
    }

    void clear() {
        tr.assign(1, Node());
    }

    int id(char c) const {
        int x = c - F;
        assert(0 <= x && x < A);
        return x;
    }

    void insert(const string& s) {
        int u = 0;
        ++tr[u].ps;
        for (char c : s) {
            int v = id(c);
            if (!tr[u].to[v]) {
                tr[u].to[v] = tr.size();
                tr.emplace_back();
            }
            u = tr[u].to[v];
            ++tr[u].ps;
        }
        ++tr[u].ed;
    }

    bool erase(const string& s) {
        int u = 0;
        vector<int> p{0};
        for (char c : s) {
            int v = id(c);
            if (!tr[u].to[v]) return false;
            u = tr[u].to[v];
            p.push_back(u);
        }
        if (!tr[u].ed) return false;
        --tr[u].ed;
        for (int v : p) --tr[v].ps;
        return true;
    }

    int ask(const string& s) const {
        int u = 0;
        for (char c : s) {
            int v = id(c);
            if (!tr[u].to[v]) return 0;
            u = tr[u].to[v];
        }
        return tr[u].ed;
    }

    int askPre(const string& s) const {
        int u = 0;
        for (char c : s) {
            int v = id(c);
            if (!tr[u].to[v]) return 0;
            u = tr[u].to[v];
        }
        return tr[u].ps;
    }

    int askMax() const {
        int r = 0;
        auto dfs = [&](auto&& go, int u, int d) -> void {
            d += tr[u].ed > 0;
            r = max(r, d);
            for (int v : tr[u].to)
                if (v && tr[v].ps) go(go, v, d);
        };
        dfs(dfs, 0, 0);
        return r;
    }

};
