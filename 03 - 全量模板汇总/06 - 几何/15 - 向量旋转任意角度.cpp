Point<ld> rotate(Point<ld> p, ld rad) {
    return {p.x * cos(rad) - p.y * sin(rad), p.x * sin(rad) + p.y * cos(rad)};
}
