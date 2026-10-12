Pd center2(Pd p1, Pd p2, Pd p3) { // 内心
    #define atan2(p) atan2(p.y, p.x) // 注意先后顺序
    Line<ld> U = {p1, {}}, V = {p2, {}};
    ld m, n, alpha;
    m = atan2((p2 - p1));
    n = atan2((p3 - p1));
    alpha = (m + n) / 2;
    U.b = {p1.x + cos(alpha), p1.y + sin(alpha)};
    m = atan2((p1 - p2));
    n = atan2((p3 - p2));
    alpha = (m + n) / 2;
    V.b = {p2.x + cos(alpha), p2.y + sin(alpha)};
    return lineIntersection(U, V);
}
