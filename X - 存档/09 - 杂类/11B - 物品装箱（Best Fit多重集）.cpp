/*
用途：按输入顺序，把物品放入“能容纳它且剩余容量最小”的箱子（Best Fit）。
说明：这是指定在线策略，不保证最少箱数；O(n log n)。
*/
template <class T> int binPack(const vector<T> &a, T c) {
    multiset<T> s;
    for (T x : a) {
        if (x > c) return -1;
        auto p = s.lower_bound(x);
        if (p == s.end()) s.insert(c - x);
        else {
            T y = *p;
            s.erase(p);
            s.insert(y - x);
        }
    }
    return s.size();
}
