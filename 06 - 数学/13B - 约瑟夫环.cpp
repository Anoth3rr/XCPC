/*
用途：n 人（0..n-1）从 0 开始，每次数 k 个并删除，求最后幸存者。
复杂度：O(n)，适合 n 可线性枚举；返回 0-indexed 或 1-indexed 答案。
*/

inline int joseph(int n, int stp) {
    assert(n >= 1 && stp >= 1);
    int ans = 0;
    for (int siz = 2; siz <= n; ++siz) ans = (ans + stp) % siz;
    return ans;
}

inline int joseph1(int n, int stp) {
    return joseph(n, stp) + 1;
}
