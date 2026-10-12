bool onPlane(P3 p1, P3 p2, P3 p3, P3 p4) { // 四点是否共面
    ld val = dot(getVec({p1, p2, p3}), p4 - p1);
    return sign(val) == 0;
}
