struct hash_pair {
    template <class A, class B>
    size_t operator()(const pair<A, B> &p) const {
        return hash<A>{}(p.first) ^ hash<B>{}(p.second);
    }
};
unordered_set<pair<int, int>, hash_pair> S;
