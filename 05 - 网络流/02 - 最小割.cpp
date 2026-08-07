/*
用途：S-T 最小割值及源点侧点集恢复。
依赖：01A - 最大流（Dinic）的 MaxFlow。
约定：点编号 0..n-1；solve 会从初始容量开始计算，可重复调用。
复杂度：与 Dinic 一致。
*/

template <class Cap = int> struct MinCut {
    struct Result {
        Cap val;
        vector<char> sd;
    };

    int n;
    MaxFlow<Cap> mf;

    explicit MinCut(int n) : n(n), mf(n) {}

    int add(int u, int v, Cap c) {
        return mf.add(u, v, c);
    }

    Result solve(int s, int t) {
        mf.reset();
        Cap val = mf.flow(s, t);
        return {val, mf.askCut(s)};
    }
};
