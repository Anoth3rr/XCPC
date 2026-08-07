/*
用途：把“恰选 k 个”的凸/凹离散优化转成带惩罚 DP。
wqsMin：f(c) 最小化 base + c*cnt；wqsMax：f(c) 最大化 base - c*cnt。
要求：惩罚 c 增大时 cnt 单调不增，且同值时 f 必须优先返回更大的 cnt。
*/
template <class T, class F>
T wqsMin(T l, T r, int k, F f) {
    while (l < r) {
        T m = l + (r - l + 1) / 2;
        auto p = f(m);
        if (p.second >= k) l = m;
        else r = m - 1;
    }
    return f(l).first - l * static_cast<T>(k);
}

template <class T, class F>
T wqsMax(T l, T r, int k, F f) {
    while (l < r) {
        T m = l + (r - l + 1) / 2;
        auto p = f(m);
        if (p.second >= k) l = m;
        else r = m - 1;
    }
    return f(l).first + l * static_cast<T>(k);
}
