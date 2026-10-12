using u64 = uint64_t;
struct myhash {
    static u64 hash(u64 x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(u64 x) const {
        static const u64 h = chrono::steady_clock::now().time_since_epoch().count();
        return hash(x + h);
    }
    size_t operator()(pair<u64, u64> x) const {
        static const u64 h = chrono::steady_clock::now().time_since_epoch().count();
        return hash(x.first + h) ^ (hash(x.second + h) >> 1);
    }
};
unordered_map<int, int, myhash> mp;
