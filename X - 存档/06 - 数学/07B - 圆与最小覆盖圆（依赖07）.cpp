/*
依赖：07 中的 Real、P、Point/Line、dot/cross/square/rotate。
用途：圆与直线/圆的交点、三点外接圆、随机增量最小覆盖圆。
浮点比较统一使用 EP；重合圆有无穷交点时 interCC 返回空数组，需按题意另判。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
constexpr Real EP = 1e-12L;
struct Circle {
    P o;
    Real r = -1;
};

inline bool inCir(const Circle& c, const P& p) {
    return c.r >= 0 && square(p - c.o) <= c.r * c.r + EP;
}

inline Circle diaCir(const P& a, const P& b) {
    P o = (a + b) / Real(2);
    return {o, sqrtl(square(a - b)) / 2};
}

inline Circle cir3(const P& a, const P& b, const P& c) {
    P x = b - a, y = c - a;
    Real d = 2 * cross(x, y);
    if (fabsl(d) <= EP) {
        Circle r = diaCir(a, b);
        for (auto [u, v] : {pair<P, P>{a, c}, pair<P, P>{b, c}}) {
            Circle q = diaCir(u, v);
            if (q.r > r.r) r = q;
        }
        return r;
    }
    P o = a + P((y.y * square(x) - x.y * square(y)) / d,
                (x.x * square(y) - y.x * square(x)) / d);
    return {o, sqrtl(square(o - a))};
}

inline vector<P> interCL(const Circle& c, const Line<Real>& l) {
    P d = l.b - l.a;
    Real z = square(d);
    assert(z > EP);
    P h = l.a + d * (dot(c.o - l.a, d) / z);
    Real q = c.r * c.r - square(h - c.o);
    if (q < -EP) return {};
    if (q <= EP) return {h};
    P v = d * sqrtl(q / z);
    return {h - v, h + v};
}

inline vector<P> interCC(const Circle& a, const Circle& b) {
    P d = b.o - a.o;
    Real z = sqrtl(square(d));
    if (z <= EP) return {};
    if (z > a.r + b.r + EP || z < fabsl(a.r - b.r) - EP) return {};
    Real x = (z * z + a.r * a.r - b.r * b.r) / (2 * z);
    Real h2 = a.r * a.r - x * x;
    P v = d / z;
    P p = a.o + v * x;
    if (h2 <= EP) return {p};
    P q = rotate(v) * sqrtl(max<Real>(0, h2));
    return {p - q, p + q};
}

inline Circle minCir(vector<P> p, int sd = 712367821) {
    if (p.empty()) return {{0, 0}, 0};
    shuffle(p.begin(), p.end(), mt19937_64(sd));
    Circle c{p[0], 0};
    for (int i = 1; i < p.size(); ++i) if (!inCir(c, p[i])) {
        c = {p[i], 0};
        for (int j = 0; j < i; ++j) if (!inCir(c, p[j])) {
            c = diaCir(p[i], p[j]);
            for (int k = 0; k < j; ++k)
                if (!inCir(c, p[k])) c = cir3(p[i], p[j], p[k]);
        }
    }
    return c;
}
