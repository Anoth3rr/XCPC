/*
用途：单调谓词的边界二分。
约定：区间均为闭区间 [l, r]。first 要求 f 从 false 单调变为 true；
last 要求 f 从 true 单调变为 false。两者均不要求端点满足谓词，找不到时返回 nullopt。
复杂度：O(log(right - left + 1))。
*/
template <class I, class F> optional<I> first(I l, I r, F f) {
    static_assert(std::is_integral_v<I> && std::is_signed_v<I>);
    optional<I> a;
    while (l <= r) {
        I m = l + (r - l) / 2;
        if (f(m)) {
            a = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return a;
}

template <class I, class F> optional<I> last(I l, I r, F f) {
    static_assert(std::is_integral_v<I> && std::is_signed_v<I>);
    optional<I> a;
    while (l <= r) {
        I m = l + (r - l) / 2;
        if (f(m)) {
            a = m;
            l = m + 1;
        } else {
            r = m - 1;
        }
    }
    return a;
}
