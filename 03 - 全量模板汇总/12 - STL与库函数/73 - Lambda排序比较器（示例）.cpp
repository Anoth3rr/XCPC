sort(a.begin(), a.end(), [](int x, int y) {
    if (x % 10 != y % 10) return x % 10 < y % 10;
    return x < y;
});
