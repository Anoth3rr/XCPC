/*
用途：统计严格逆序对数，即 i < j 且 values[i] > values[j] 的对数。
约定：Compare 默认 less<T>；传入的比较器应满足严格弱序。
复杂度：O(n log n) 时间、O(n) 额外空间；输入数组不被修改。
*/

template <class T, class Cmp = less<T>> int invCnt(vector<T> a, Cmp cmp = Cmp{}) {
    vector<T> buf(a.size());

    auto mg = [&](auto&& go, int l, int r) -> int {
        if (r - l <= 1) return 0;
        int mid = l + (r - l) / 2;
        int ans = go(go, l, mid) + go(go, mid, r);

        int i = l, j = mid, p = l;
        while (i < mid || j < r) {
            if (j == r || (i < mid && !cmp(a[j], a[i]))) {
                buf[p++] = a[i++];
            } else {
                ans += mid - i;
                buf[p++] = a[j++];
            }
        }
        for (int id = l; id < r; ++id) a[id] = buf[id];
        return ans;
    };

    return mg(mg, 0, a.size());
}
