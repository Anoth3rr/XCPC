auto operator<=>(const State &o) const {
    return tie(y, x, z) <=> tie(o.y, o.x, o.z);
}
bool operator==(const State &) const = default;
