using ld = long double;
const ld PI = acos(-1);
const ld EPS = 1e-7;
const ld INF = numeric_limits<ld>::max();
#define cc(x) cout << fixed << setprecision(x);

ld fgcd(ld x, ld y) { // 实数域gcd
    return abs(y) < EPS ? abs(x) : fgcd(y, fmod(x, y));
}
template<class T, class S> bool equal(T x, S y) {
    return -EPS < x - y && x - y < EPS;
}
template<class T> int sign(T x) {
    if (-EPS < x && x < EPS) return 0;
    return x < 0 ? -1 : 1;
}
