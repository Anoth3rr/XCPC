Point<ld> rotateDegrees(Point<ld> a, ld d) {
    ld t = d * acosl(-1.L) / 180;
    return {a.x * cosl(t) - a.y * sinl(t), a.x * sinl(t) + a.y * cosl(t)};
}
