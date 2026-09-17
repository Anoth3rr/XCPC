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
        return x;
    }

    void insert(const string &s) {
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

    bool erase(const string &s) {
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

    int ask(const string &s) const {
        int u = 0;
        for (char c : s) {
            int v = id(c);
            if (!tr[u].to[v]) return 0;
            u = tr[u].to[v];
        }
        return tr[u].ed;
    }

    int getpre(const string &s) const {
        int u = 0;
        for (char c : s) {
            int v = id(c);
            if (!tr[u].to[v]) return 0;
            u = tr[u].to[v];
        }
        return tr[u].ps;
    }

    int getmax() const {
        int r = 0;
        auto dfs = [&](auto &&go, int u, int d) -> void {
            d += tr[u].ed > 0;
            r = max(r, d);
            for (int v : tr[u].to)
                if (v && tr[v].ps) go(go, v, d);
        };
        dfs(dfs, 0, 0);
        return r;
    }
};
