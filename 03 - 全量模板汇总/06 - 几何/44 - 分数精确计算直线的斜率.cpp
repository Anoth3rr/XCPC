template<class T> Frac<T> slopeEx(Pt p1, Pt p2) {
    Frac<T> U = p1.y - p2.y;
    Frac<T> V = p1.x - p2.x;
    return U / V; // 调用分数精确计算
}
