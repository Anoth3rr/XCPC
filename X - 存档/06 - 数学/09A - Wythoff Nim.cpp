/*
用途：判断 Wythoff Nim（两堆，每步可取一堆任意个或两堆各取相同正数）的必败态。
结论：设 a<=b、k=b-a，则 (a,b) 为必败态当且仅当 a=floor(k*phi)。
浮点实现适合 a,b 约 1e15 及以内；更大范围需额外做精确无理数比较。
*/
using ld = long double;

namespace WythoffNim {
inline int flrPhi(int k) {
    static const ld phi = (1.0L + sqrtl(5.0L)) / 2.0L;
    return floorl(ld(k) * phi);
}

inline pair<int, int> lose(int k) {
    int fst = flrPhi(k);
    return {fst, fst + k};
}

inline bool ask(int fst, int b) {
    if (fst > b) swap(fst, b);
    return fst == flrPhi(b - fst);
}
}  // namespace WythoffNim
