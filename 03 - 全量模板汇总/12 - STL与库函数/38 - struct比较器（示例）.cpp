struct State {
    string x, y;
    int z;

    bool operator<(const State& o) const {
        return tie(x, y, z) < tie(o.x, o.y, o.z);
    }
    bool operator==(const State& o) const {
        return tie(x, y, z) == tie(o.x, o.y, o.z);
    }
};
