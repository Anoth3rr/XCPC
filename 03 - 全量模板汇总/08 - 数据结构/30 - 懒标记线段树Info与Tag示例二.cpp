struct Tag {
    Z add{}, mul{};
    Tag(Z a = 0, Z m = 1) : add(a), mul(m) {}
    Tag &operator+=(const Tag &o) {
        add = add * o.mul + o.add;
        mul = mul * o.mul;
        return *this;
    }
};

struct Info {
    Z val{};
    int len{};

    Info() = default;
    Info(Z v, int len = 1) : val(v), len(len) {}

    Info operator+(const Info &o) const {
        return {val + o.val, len + o.len};
    }

    Info &operator+=(const Tag &tag) {
        val = val * tag.mul + tag.add * len;
        return *this;
    }
};
