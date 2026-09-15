/*
用途：维护整数定义域上的凸分段线性函数。
接口：addAMX 加 max(a-x,0)，addXMA 加 max(x-a,0)，addAbs 加绝对值；
      preMin/sufMin 做前缀/后缀最小化，shift 做平移或滑动窗口最小化，ask 求函数值。
约定：函数初始为 0；preMin 后 f(x)=min_{y<=x} old f(y)，sufMin 对称；
      shift(a,b) 要求 a<=b，表示 f_new(x)=min_{x-b<=y<=x-a} old f(y)。
复杂度：每次修改 O(log n)，最小值和最优区间 O(1)；ask 会复制堆，O(n log n)。
*/
template <class T = int> struct SlopeTrick {
    using i128 = __int128_t;
    using Q = priority_queue<T>;
    using R = priority_queue<T, vector<T>, greater<T>>;

    Q l;
    R r;
    T dl = 0, dr = 0, mn = 0;

    T topL() const {
        assert(!l.empty());
        return l.top() + dl;
    }

    T topR() const {
        assert(!r.empty());
        return r.top() + dr;
    }

    void pushL(T x) {
        l.push(x - dl);
    }

    void pushR(T x) {
        r.push(x - dr);
    }

    void addAll(T x) {
        mn += x;
    }

    // f(x) += max(x-a, 0)
    void addXMA(T a) {
        if (!l.empty() && topL() > a) {
            T x = topL();
            mn += x - a;
            l.pop();
            pushL(a);
            pushR(x);
        } else {
            pushR(a);
        }
    }

    // f(x) += max(a-x, 0)
    void addAMX(T a) {
        if (!r.empty() && topR() < a) {
            T x = topR();
            mn += a - x;
            r.pop();
            pushR(a);
            pushL(x);
        } else {
            pushL(a);
        }
    }

    void addAbs(T a) {
        addAMX(a);
        addXMA(a);
    }

    void clearL() {
        l = Q{};
    }

    void clearR() {
        r = R{};
    }

    void preMin() {
        clearR();
    }

    void sufMin() {
        clearL();
    }

    void shift(T a) {
        dl += a;
        dr += a;
    }

    void shift(T a, T b) {
        assert(a <= b);
        dl += a;
        dr += b;
    }

    T askMin() const {
        return mn;
    }

    pair<T, T> askArg() const {
        T x = l.empty() ? numeric_limits<T>::lowest() / 4 : topL();
        T y = r.empty() ? numeric_limits<T>::max() / 4 : topR();
        return {x, y};
    }

    T ask(T x) const {
        i128 z = mn;
        auto a = l;
        auto b = r;
        while (!a.empty()) {
            T y = a.top() + dl;
            if (y > x) z += i128(y) - x;
            a.pop();
        }
        while (!b.empty()) {
            T y = b.top() + dr;
            if (x > y) z += i128(x) - y;
            b.pop();
        }
        assert(numeric_limits<T>::lowest() <= z && z <= numeric_limits<T>::max());
        return static_cast<T>(z);
    }
};
