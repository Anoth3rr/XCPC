ld area(Point<ld> a, Point<ld> b, Point<ld> c) {
    return abs(cross(b, c, a)) / 2;
}
