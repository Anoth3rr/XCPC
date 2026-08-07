/*
用途：求每个位置左侧（prev）或右侧（next）最近的满足大小关系的位置。

约定：
- 输入 vector 为 0-indexed，返回值也是下标；不存在时返回 -1。
- less / greater 为严格比较，less_equal / greater_equal 为非严格比较。
- 求所有子数组最小值/最大值贡献时，左右两侧应一边用严格比较、另一边用非严格比较，
  以保证重复元素只计算一次。
*/

template <class T> struct MonoStack {
  private:
    template <class F> static vector<int> prev(const vector<T> &a, F f) {
        int n = a.size();
        vector<int> ans(n), st;
        st.reserve(n);
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && !f(a[st.back()], a[i])) st.pop_back();
            ans[i] = st.empty() ? -1 : st.back();
            st.push_back(i);
        }
        return ans;
    }

    template <class F> static vector<int> next(const vector<T> &a, F f) {
        int n = a.size();
        vector<int> ans(n), st;
        st.reserve(n);
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && !f(a[st.back()], a[i])) st.pop_back();
            ans[i] = st.empty() ? -1 : st.back();
            st.push_back(i);
        }
        return ans;
    }

  public:
    static vector<int> preL(const vector<T> &a) {
        return prev(a, [](const T &x, const T &y) { return x < y; });
    }

    static vector<int> preLE(const vector<T> &a) {
        return prev(a, [](const T &x, const T &y) { return x <= y; });
    }

    static vector<int> preG(const vector<T> &a) {
        return prev(a, [](const T &x, const T &y) { return x > y; });
    }

    static vector<int> preGE(const vector<T> &a) {
        return prev(a, [](const T &x, const T &y) { return x >= y; });
    }

    static vector<int> nxtL(const vector<T> &a) {
        return next(a, [](const T &x, const T &y) { return x < y; });
    }

    static vector<int> nxtLE(const vector<T> &a) {
        return next(a, [](const T &x, const T &y) { return x <= y; });
    }

    static vector<int> nxtG(const vector<T> &a) {
        return next(a, [](const T &x, const T &y) { return x > y; });
    }

    static vector<int> nxtGE(const vector<T> &a) {
        return next(a, [](const T &x, const T &y) { return x >= y; });
    }
};
