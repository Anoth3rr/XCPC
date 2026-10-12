ld planeCos(Plane s1, Plane s2) {
    P3 U = getVec(s1), V = getVec(s2);
    return dot(U, V) / len(U) / len(V);
}
