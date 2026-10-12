struct hash_state {
    size_t operator()(const State& v) const {
        constexpr size_t C = 0x9e3779b97f4a7c15ULL;

        size_t h = hash<string>{}(v.x);
        h ^= hash<string>{}(v.y) + C + (h << 6) + (h >> 2);
        h ^= hash<int>{}(v.z) + C + (h << 6) + (h >> 2);
        return h;
    }
};
