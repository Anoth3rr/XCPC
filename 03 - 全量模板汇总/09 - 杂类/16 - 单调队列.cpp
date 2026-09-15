/*
用途：滑动窗口最大/最小值。
用法：MonoQueue<int> q(k) 默认维护最大值；MonoQueue<int, greater<int>> 维护最小值。
接口：push(x) 加入下一个位置；ask() 在窗口已满时返回窗口答案，否则返回 nullopt。
复杂度：每个元素至多进出队一次，总 O(n)，空间 O(k)。
*/

template <class T, class C = less<T>> struct MonoQueue {
    int k, p;
    C cmp;
    deque<pair<T, int>> q;

    explicit MonoQueue(int k) : k(k), p(-1) {
        assert(k > 0);
    }

    void push(const T& x) {
        ++p;
        while (!q.empty() && cmp(q.back().first, x)) q.pop_back();
        q.push_back({x, p});
        while (q.front().second <= p - k) q.pop_front();
    }

    optional<T> ask() const {
        if (p + 1 < k) return nullopt;
        return q.front().first;
    }
};
