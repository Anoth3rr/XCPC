/*
用途：带自定义聚合信息的键值 FHQ Treap 多重集。

Info 需支持默认构造与 operator+；MakeInfo(key, count) 生成单键贡献。
当前接口聚焦 insert/erase/ask（全局聚合）；若需要 rank/kth/前驱后继，使用 10A。
*/
template <class T, class F> class FHQTreap {
  private:
    struct Node {
        int l = 0, r = 0;
        int key = 0, cnt = 0;
        unsigned pri = 0;
        T val;
    };

    vector<Node> tr;
    int rt = 0;
    F mk;
    mt19937 rng;

    int node(int key, int cnt = 1) {
        tr.push_back(Node());
        int p = tr.size() - 1;
        tr[p].key = key;
        tr[p].cnt = cnt;
        tr[p].pri = rng();
        tr[p].val = mk(key, cnt);
        return p;
    }

    void pull(int p) {
        if (!p) return;
        tr[p].val = tr[tr[p].l].val + mk(tr[p].key, tr[p].cnt) + tr[tr[p].r].val;
    }

    void split(int p, int key, int &x, int &y) {
        if (!p) {
            x = y = 0;
            return;
        }
        if (tr[p].key <= key) {
            x = p;
            split(tr[p].r, key, tr[p].r, y);
            pull(x);
        } else {
            y = p;
            split(tr[p].l, key, x, tr[p].l);
            pull(y);
        }
    }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        if (tr[x].pri < tr[y].pri) {
            tr[x].r = merge(tr[x].r, y);
            pull(x);
            return x;
        } else {
            tr[y].l = merge(x, tr[y].l);
            pull(y);
            return y;
        }
    }

  public:
    FHQTreap(F f)
        : tr(1), mk(f),
          rng(chrono::steady_clock::now().time_since_epoch().count()) {}

    bool askEmp() const {
        return rt == 0;
    }

    void insert(int key, int cnt = 1) {
        int a, b, c;
        split(rt, key - 1, a, b);
        split(b, key, b, c);
        if (b) {
            tr[b].cnt += cnt;
            pull(b);
        } else {
            b = node(key, cnt);
        }
        rt = merge(merge(a, b), c);
    }

    void erase(int key, int cnt = 1) {
        int a, b, c;
        split(rt, key - 1, a, b);
        split(b, key, b, c);
        if (b) {
            if (tr[b].cnt > cnt) {
                tr[b].cnt -= cnt;
                pull(b);
            } else {
                b = merge(tr[b].l, tr[b].r);
            }
        }
        rt = merge(merge(a, b), c);
    }

    T ask() const {
        return tr[rt].val;
    }
};
