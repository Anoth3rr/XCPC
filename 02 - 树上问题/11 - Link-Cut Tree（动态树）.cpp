/*
用途：Link-Cut Tree，维护动态森林上的路径和与路径最大值。

接口（点编号 1..n）：
- modify(u, v)：修改点权。
- ask(u, v)：判断是否连通。
- link(u, v)：连一条边；若会成环则返回 false。
- cut(u, v)：删除边 u-v；若该边不存在则返回 false。
- askSum(u, v) / askMax(u, v)：返回路径点权和/最大值，调用前保证两点连通。

T 应为支持加法、比较和 numeric_limits 的数值类型，默认 int。该版本维护点权，
边权可通过“为每条边新建一个点并把权值放在该点”转换。
*/

template <class T = int>
struct LCT {
    int n;
    vector<array<int, 2>> son;
    vector<int> fa;
    vector<char> tag;
    vector<T> val, sum, mx;

    explicit LCT(int n, T v = T{})
        : n(n), son(n + 1), fa(n + 1), tag(n + 1), val(n + 1, v),
          sum(n + 1, v), mx(n + 1, v) {
        sum[0] = T{};
        mx[0] = numeric_limits<T>::lowest();
    }

    bool isRoot(int x) const {
        int p = fa[x];
        return p == 0 || (son[p][0] != x && son[p][1] != x);
    }

    void pull(int x) {
        sum[x] = sum[son[x][0]] + val[x] + sum[son[x][1]];
        mx[x] = max(val[x], max(mx[son[x][0]], mx[son[x][1]]));
    }

    void rev(int x) {
        if (!x) return;
        swap(son[x][0], son[x][1]);
        tag[x] ^= 1;
    }

    void push(int x) {
        if (!x || !tag[x]) return;
        rev(son[x][0]);
        rev(son[x][1]);
        tag[x] = 0;
    }

    void pushAll(int x) {
        static vector<int> pth;
        pth.clear();
        pth.push_back(x);
        for (int u = x; !isRoot(u); u = fa[u]) pth.push_back(fa[u]);
        for (int i = pth.size(); i--;) push(pth[i]);
    }

    void rotate(int x) {
        int p = fa[x], g = fa[p];
        int sd = (son[p][1] == x);
        int mid = son[x][sd ^ 1];
        if (!isRoot(p)) son[g][son[g][1] == p] = x;
        fa[x] = g;
        son[x][sd ^ 1] = p;
        fa[p] = x;
        son[p][sd] = mid;
        if (mid) fa[mid] = p;
        pull(p);
        pull(x);
    }

    void splay(int x) {
        pushAll(x);
        while (!isRoot(x)) {
            int p = fa[x], g = fa[p];
            if (!isRoot(p)) {
                if ((son[p][0] == x) == (son[g][0] == p)) rotate(p);
                else rotate(x);
            }
            rotate(x);
        }
        pull(x);
    }

    void access(int x) {
        int lst = 0;
        for (int u = x; u; u = fa[u]) {
            splay(u);
            son[u][1] = lst;
            pull(u);
            lst = u;
        }
        splay(x);
    }

    void mkRoot(int x) {
        access(x);
        rev(x);
    }

    int root(int x) {
        access(x);
        while (son[x][0]) {
            push(x);
            x = son[x][0];
        }
        splay(x);
        return x;
    }

    bool ask(int u, int v) {
        return u == v || root(u) == root(v);
    }

    bool link(int u, int v) {
        mkRoot(u);
        if (root(v) == u) return false;
        fa[u] = v;
        return true;
    }

    bool cut(int u, int v) {
        mkRoot(u);
        access(v);
        if (son[v][0] != u || son[u][1] != 0) return false;
        son[v][0] = 0;
        fa[u] = 0;
        pull(v);
        return true;
    }

    void split(int u, int v) {
        mkRoot(u);
        access(v);
    }

    void modify(int u, T v) {
        access(u);
        val[u] = v;
        pull(u);
    }

    T askSum(int u, int v) {
        split(u, v);
        return sum[v];
    }

    T askMax(int u, int v) {
        split(u, v);
        return mx[v];
    }
};
