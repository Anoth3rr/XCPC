struct hash_vector {
    template<class T>
    size_t operator()(const vector<T>& a) const noexcept {
        size_t h = 0;
        for (const auto &it : a) {
            h ^= hash<T>{}(it) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};
