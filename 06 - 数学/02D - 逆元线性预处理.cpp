/*
用途：预处理 1..n 在质数模下的逆元。
用法：auto iv = invAll<Z>(n)，其中 Z 可为任意 MInt<P>。
前提：Z::askMod() 为质数且 n < mod；复杂度 O(n)。
*/
template <class Z> vector<Z> invAll(int n) {
    assert(n >= 0 && n < Z::askMod());
    vector<Z> iv(n + 1);
    if (n) iv[1] = Z(1);
    int p = Z::askMod();
    for (int i = 2; i <= n; ++i) iv[i] = Z(p - p / i) * iv[p % i];
    return iv;
}
