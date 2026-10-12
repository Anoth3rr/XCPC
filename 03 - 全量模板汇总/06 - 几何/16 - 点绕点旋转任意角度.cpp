Point<ld> rotate(Point<ld> a, Point<ld> b, ld rad) {
    ld x = (a.x - b.x) * cos(rad) + (a.y - b.y) * sin(rad) + b.x;
    ld y = (b.x - a.x) * sin(rad) + (a.y - b.y) * cos(rad) + b.y;
    return {x, y};
}
