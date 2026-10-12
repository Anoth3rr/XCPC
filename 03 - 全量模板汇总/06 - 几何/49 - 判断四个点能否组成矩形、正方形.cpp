template<class T> int isSquare(vector<Pt> x) {
    sort(x.begin(), x.end());
    if (equal(dis(x[0], x[1]), dis(x[2], x[3])) && sign(dis(x[0], x[1])) &&
        equal(dis(x[0], x[2]), dis(x[1], x[3])) && sign(dis(x[0], x[2])) &&
        lineParallel(Lt{x[0], x[1]}, Lt{x[2], x[3]}) &&
        lineParallel(Lt{x[0], x[2]}, Lt{x[1], x[3]}) &&
        lineVertical(Lt{x[0], x[1]}, Lt{x[0], x[2]})) {
        return equal(dis(x[0], x[1]), dis(x[0], x[2])) ? 2 : 1;
    }
    return 0;
}
