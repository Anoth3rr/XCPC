Pd lineIntersection(Ld l1, Ld l2) {
    ld val = cross(l2.b - l2.a, l1.a - l2.a) / cross(l2.b - l2.a, l1.a - l1.b);
    return l1.a + (l1.b - l1.a) * val;
}
