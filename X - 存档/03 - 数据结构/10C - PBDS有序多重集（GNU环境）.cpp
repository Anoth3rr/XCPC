template <class Key> struct OSet {
    using P = pair<Key, int>;
    using Tree = __gnu_pbds::tree<P, __gnu_pbds::null_type, less<P>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
    Tree t;
    int id = 0;

    int insert(const Key &x) {
        int k = ++id;
        t.insert({x, k});
        return k;
    }

    bool erase(const Key &x, int k) {
        return t.erase({x, k});
    }

    bool erase(const Key &x) {
        auto it = t.lower_bound({x, 0});
        if (it == t.end() || it->first != x) return false;
        t.erase(it);
        return true;
    }

    int askSz() const {
        return t.size();
    }
    bool askEmp() const {
        return t.empty();
    }
    void clear() {
        t.clear(), id = 0;
    }
    int askCnt(const Key &x) const {
        return t.order_of_key({x, numeric_limits<int>::max()}) - t.order_of_key({x, 0});
    }
    int askRank(const Key &x) const {
        return t.order_of_key({x, 0}) + 1;
    }

    optional<Key> askKth(int k) const {
        if (k < 1 || k > askSz()) return nullopt;
        return t.find_by_order(k - 1)->first;
    }

    optional<Key> askPre(const Key &x) const {
        auto it = t.lower_bound({x, 0});
        if (it == t.begin()) return nullopt;
        return (--it)->first;
    }

    optional<Key> askNxt(const Key &x) const {
        auto it = t.upper_bound({x, numeric_limits<int>::max()});
        return it == t.end() ? nullopt : optional<Key>(it->first);
    }

    optional<Key> askGE(const Key &x) const {
        auto it = t.lower_bound({x, 0});
        return it == t.end() ? nullopt : optional<Key>(it->first);
    }
};
