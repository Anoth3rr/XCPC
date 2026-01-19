struct myhash {
    using ull = unsigned long long;
    static ull hash(ull x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    int operator()(ull x) const {
        static const ull SEED = chrono::steady_clock::now().time_since_epoch().count();
        return hash(x + SEED);
    }
    int operator()(pair<ull, ull> x) const {
        static const ull SEED = chrono::steady_clock::now().time_since_epoch().count();
        return hash(x.first + SEED) ^ (hash(x.second + SEED) >> 1);
    }
};