/*
用途：O(n) 构造笛卡尔树；中序遍历为原下标顺序，同时满足堆性质。
模板：C=less<T> 为小根树，C=greater<T> 为大根树；E=true 时相等元素中右侧在上层。
约定：输入及 rt/lc/rc/fa 均为 0-indexed，空节点为 -1。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
template <class T, class C = less<T>, bool E = false>
struct CartTree {
    int rt = -1;
    vector<int> lc, rc, fa;
    C c;

    CartTree() = default;
    explicit CartTree(const vector<T>& a, C c = {}) : c(c) { build(a); }

    void build(const vector<T>& a) {
        int n = a.size();
        rt = -1;
        lc.assign(n, -1);
        rc.assign(n, -1);
        fa.assign(n, -1);
        vector<int> s;
        s.reserve(n);
        for (int i = 0; i < n; ++i) {
            int x = -1;
            while (!s.empty()) {
                int j = s.back();
                bool f = c(a[i], a[j]);
                if constexpr (E) f = f || !c(a[j], a[i]);
                if (!f) break;
                x = j;
                s.pop_back();
            }
            if (s.empty()) rt = i;
            else fa[i] = s.back(), rc[s.back()] = i;
            if (x != -1) fa[x] = i, lc[i] = x;
            s.push_back(i);
        }
    }
};
