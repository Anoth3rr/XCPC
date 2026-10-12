struct Tag {
    int v{};
    Tag(int x = 0) : v(x) {}
    Tag &operator+=(const Tag &o) {
        v += o.v;
        return *this;
    }
};

struct Info {
    int val{}, len{};

    Info() = default;
    Info(int v, int len = 1) : val(v), len(len) {}

    Info operator+(const Info &o) const {
        return {val + o.val, len + o.len};
    }

    Info &operator+=(const Tag &tag) {
        val += tag.v * len;
        return *this;
    }
};
