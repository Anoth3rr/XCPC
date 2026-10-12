struct Tag {
    bool f{};
    int v{}, t{};
    Tag(int x = 0) : f(O), v(x), t(0) {}
    Tag(bool f, int t) : f(f), t(t) {}
    Tag &operator+=(const Tag &o) {
        if(o.f) {
            f = 1;
            t = o.t;
            v = o.v;
        } else {
            v += o.v;
        }
    }
};
