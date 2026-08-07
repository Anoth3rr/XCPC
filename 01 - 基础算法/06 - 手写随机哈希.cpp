using u64 = uint64_t;

// 抗 hack 哈希

struct Hash {

    static u64 hash(u64 x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    static const u64 sd;

    static u64 hash(const string &x) {
        return std::hash<string>{}(x);
    }

    template <class T> size_t operator()(T x) const {
        return hash(u64(x) + sd);
    }
    size_t operator()(const string &s) const {
        u64 h = 0;
        for (char c : s) h = h * 131 + c;
        return hash(h + sd);
    }
    template <class T1, class T2> size_t operator()(const pair<T1, T2> &p) const {
        return operator()(p.first) ^ (operator()(p.second) >> 1);
    }
    template <class T> size_t operator()(const vector<T> &v) const {
        u64 h = 0;
        for (auto &x : v) {
            h = h * 131 + operator()(x);
        }
        return hash(h);
    }
};
const u64 Hash::sd = chrono::steady_clock::now().time_since_epoch().count();

// struct ，需要在结构体中重载等于号
struct Node {
    string x, y;
    int z;
    friend bool operator==(const Node &a, const Node &b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};
// 结构体版本；也可以把这个 operator() 作为重载加入上面的 Hash。
struct NodeHash {
    size_t operator()(const Node &p) const {
        Hash h;
        return h(p.x) ^ (h(p.y) << 1) ^ (h(p.z) << 2);
    }
};
