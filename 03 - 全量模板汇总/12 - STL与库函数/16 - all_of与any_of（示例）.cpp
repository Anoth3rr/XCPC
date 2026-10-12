bool all_positive = all_of(a.begin(), a.end(), [](int v) {
    return v > 0;
});
bool has_negative = any_of(a.begin(), a.end(), [](int v) {
    return v < 0;
});
