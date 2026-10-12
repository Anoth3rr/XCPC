struct State {
    string x, y;
    int z;
    bool operator==(const State& o) const {
        return tie(x, y, z) == tie(o.x, o.y, o.z);
    }
};
struct hash_state {
    size_t operator()(const State &p) const {
        return hash<string>{}(p.x) ^ hash<string>{}(p.y) ^ hash<int>{}(p.z);
    }
};
unordered_map<State, int, hash_state> mp;
