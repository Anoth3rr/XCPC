/*
用途：大图的连续内存邻接表；与 vector<vector<Edge>> 是同一存图问题的内存/缓存特化分支。
点编号 0..n-1，head[u] 起沿 edge[i].next 遍历。预先 reserve(m) 可避免扩容。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/

template <class W = int> struct Graph {
    struct Edge {
        int v, nxt;
        W w;
    };
    int n;
    vector<int> hd;
    vector<Edge> e;

    explicit Graph(int n, int m = 0) : n(n), hd(n, -1) {
        assert(n >= 0 && m >= 0);
        e.reserve(m);
    }

    int add(int u, int v, W w = W(1)) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        e.push_back({v, hd[u], w});
        return hd[u] = e.size() - 1;
    }

    array<int, 2> add2(int u, int v, W w = W(1)) {
        int a = add(u, v, w);
        int b = add(v, u, w);
        return {a, b};
    }
};
