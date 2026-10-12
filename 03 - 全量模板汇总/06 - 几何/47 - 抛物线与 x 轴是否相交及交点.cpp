tuple<int, ld, ld> getAns(ld a, ld b, ld c) {
    ld d = b * b - a * c * 4;
    if (d < 0.) return {0, 0, 0};
    d = sqrt(d);
    ld ans1 = -(d + b) / 2 / a;
    ld ans2 = (d - b) / 2 / a;
    if (ans1 > ans2) swap(ans1, ans2);
    if (sign(d) == 0) return {1, ans2, 0};
    return {2, ans1, ans2};
}
