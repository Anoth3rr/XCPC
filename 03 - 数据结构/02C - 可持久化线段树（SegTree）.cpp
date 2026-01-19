struct Info {
    int cnt = 0;
    ll sum = 0;
    Info operator+(const Info &o) {
        Info res;
        res.cnt = cnt + o.cnt;
        res.sum = sum + o.sum;
        return res;
    }
};

struct Node {
    Node *l = nullptr;
    Node *r = nullptr;
    Info info;
};

class SegTree {
  private:
    ll L, R;
    vector<Node *> roots;

    Node *insert(Node *t, ll l, ll r, ll p, ll v) {
        Node *x = new Node();
        if (t)
            *x = *t;
        x->info.cnt += 1;
        x->info.sum += v;
        if (r - l == 1) {
            return x;
        }
        ll mid = (l + r) >> 1;
        if (p < mid)
            x->l = insert(t ? t->l : nullptr, l, mid, p, v);
        else
            x->r = insert(t ? t->r : nullptr, mid, r, p, v);
        return x;
    }

    int clone_add(int ver, ll p, ll v) {
        if (p < L || p >= R) {
            throw runtime_error("position out of range in clone_add");
        }
        Node *root = insert(roots[ver], L, R, p, v);
        roots.push_back(root);
        return roots.size() - 1;
    }

    Info get_node(Node *t, ll l, ll r, ll x, ll y) const {
        if (!t || l >= y || r <= x)
            return Info{};
        if (l >= x && r <= y)
            return t->info;
        ll mid = (l + r) >> 1;
        return get_node(t->l, l, mid, x, y) + get_node(t->r, mid, r, x, y);
    }

    ll find_kth(Node *tl, Node *tr, ll l, ll r, int k) const {
        if (k <= 0)
            return -1;
        int tot = (tr ? tr->info.cnt : 0) - (tl ? tl->info.cnt : 0);
        if (k > tot)
            return -1;
        while (r - l > 1) {
            ll mid = (l + r) >> 1;
            int cnt = (tr && tr->l ? tr->l->info.cnt : 0) - (tl && tl->l ? tl->l->info.cnt : 0);
            if (k <= cnt) {
                tl = (tl ? tl->l : nullptr);
                tr = (tr ? tr->l : nullptr);
                r = mid;
            } else {
                k -= cnt;
                tl = (tl ? tl->r : nullptr);
                tr = (tr ? tr->r : nullptr);
                l = mid;
            }
        }
        return l;
    }

  public:
    SegTree(ll L_, ll R_) : L(L_), R(R_) {
        roots.push_back(nullptr);
    }

    int add(int ver, ll p, ll v) {
        return clone_add(ver, p, v);
    }

    Info get(int ver, ll x, ll y) const {
        if (x >= y)
            return Info{};
        return get_node(roots[ver], L, R, x, y);
    }

    ll find(int verL, int verR, int k) const {
        return find_kth(roots[verL], roots[verR], L, R, k);
    }

    int versions() const { return roots.size(); }
};