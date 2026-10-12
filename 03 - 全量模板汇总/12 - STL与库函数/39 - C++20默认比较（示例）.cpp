struct State {
    string x, y;
    int z;

    auto operator<=>(const State&) const = default;
};
