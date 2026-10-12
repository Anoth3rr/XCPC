bool planeParallel(Plane s1, Plane s2) {
    ld val = cross(getVec(s1), getVec(s2));
    return sign(val) == 0;
}
bool planeVertical(Plane s1, Plane s2) {
    ld val = dot(getVec(s1), getVec(s2));
    return sign(val) == 0;
}
