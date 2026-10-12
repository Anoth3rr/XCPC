struct Info {
    int val = -inf, len{};
    Info() = default;
    Info(int v, int len = 1) : val(v), len(len) {}

    Info operator+(const Info &o) const {
        return {max(val, o.val), len + o.len};
    }
};
