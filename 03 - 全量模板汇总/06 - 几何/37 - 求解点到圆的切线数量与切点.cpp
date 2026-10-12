pair<int, vector<Point<f64>>> tangent(Point<f64> A, Point<f64> O, f64 r) {
    vector<Point<f64>> ans; // 储存切点
    Point<f64> u = O - A;
    f64 d = sqrt(dot(u, u));
    if (d < r) {
        return {0, {}};
    } else if (sign(d - r) == 0) { // 点在圆上
        ans.push_back(u);
        return {1, ans};
    } else {
        A = O - u / d * r;
        f64 cos = r / d, sin = sqrt(1 - cos * cos);
        auto clac = [&]() -> Point<f64> {
            f64 x = A.x - O.x, y = A.y - O.y;
            return {x * cos + y * sin, (-x) * sin + y * cos};
        };
        ans.push_back(clac() + O);
        sin = -sin;
        ans.push_back(clac() + O);
        return {2, ans};
    }
}
