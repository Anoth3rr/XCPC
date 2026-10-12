struct hash_vector {
    template<class T>
    size_t operator()(const vector<T> &p) const {
        size_t h = 0;
        for (const auto &it : p) {
            h ^= hash<T>{}(it);
        }
        return h;
    }
};
unordered_map<vector<int>, int, hash_vector> mp;
