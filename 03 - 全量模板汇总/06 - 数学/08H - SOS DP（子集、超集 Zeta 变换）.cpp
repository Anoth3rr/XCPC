/*
用途：对所有 mask 计算子集和或超集和，以及对应的 Möbius 逆变换。
前提：数组长度为 2^k，元素类型 T 支持 += 和 -=。
复杂度：O(k 2^k)。常用于按位包含关系计数、容斥和子集 DP。
*/
namespace SOSDP {
inline int bits(int siz) {
    assert(siz > 0 && (siz & (siz - 1)) == 0);
    return __builtin_ctz((unsigned)siz);
}

template <class T> void zetaSub(vector<T>& a) {
    int k = bits(a.size());
    for (int i = 0; i < k; ++i)
        for (int msk = 0; msk < a.size(); ++msk)
            if (msk >> i & 1) a[msk] += a[msk ^ (1 << i)];
}

template <class T> void mobSub(vector<T>& a) {
    int k = bits(a.size());
    for (int i = 0; i < k; ++i)
        for (int msk = 0; msk < a.size(); ++msk)
            if (msk >> i & 1) a[msk] -= a[msk ^ (1 << i)];
}

template <class T> void zetaSup(vector<T>& a) {
    int k = bits(a.size());
    for (int i = 0; i < k; ++i)
        for (int msk = 0; msk < a.size(); ++msk)
            if ((msk >> i & 1) == 0) a[msk] += a[msk | (1 << i)];
}

template <class T> void mobSup(vector<T>& a) {
    int k = bits(a.size());
    for (int i = 0; i < k; ++i)
        for (int msk = 0; msk < a.size(); ++msk)
            if ((msk >> i & 1) == 0) a[msk] -= a[msk | (1 << i)];
}
}  // namespace SOSDP
