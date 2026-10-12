struct hash_pair {
    template<class A, class B>
    size_t operator()(const pair<A, B>& p) const {
        size_t h1 = hash<A>{}(p.first);
        size_t h2 = hash<B>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};
