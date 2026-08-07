/*
用途：GNU PBDS 有序多重集；FHQ Treap 的 GNU 环境高性能替代实现。
限制：仅 GCC/libstdc++ 可用，Clang+libc++、MSVC 不可用，因此与可移植的 10A 同时保留。
排名和 kth 均为 1-indexed；重复键通过唯一编号区分。
性能：默认使用宏展开后的 int；若大规模数据因此超时或超空间，可将纯下标、状态或计数局部改为 i32。
*/
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

template <class Key> struct OSet {
    using P = pair<Key, int>;
    using Tree = __gnu_pbds::tree<P, __gnu_pbds::null_type, less<P>,
                                  __gnu_pbds::rb_tree_tag,
                                  __gnu_pbds::tree_order_statistics_node_update>;
    Tree t;
    int id = 0;

    int insert(const Key& x) {
        int k = ++id;
        t.insert({x, k});
        return k;
    }

    bool erase(const Key& x, int k) { return t.erase({x, k}); }

    bool erase(const Key& x) {
        auto it = t.lower_bound({x, 0});
        if (it == t.end() || it->first != x) return false;
        t.erase(it);
        return true;
    }

    int askSz() const { return t.size(); }
    bool askEmp() const { return t.empty(); }
    void clear() { t.clear(), id = 0; }
    int askCnt(const Key& x) const {
        return t.order_of_key({x, numeric_limits<int>::max()}) - t.order_of_key({x, 0});
    }
    int askRank(const Key& x) const { return t.order_of_key({x, 0}) + 1; }

    optional<Key> askKth(int k) const {
        if (k < 1 || k > askSz()) return nullopt;
        return t.find_by_order(k - 1)->first;
    }

    optional<Key> askPre(const Key& x) const {
        auto it = t.lower_bound({x, 0});
        if (it == t.begin()) return nullopt;
        return (--it)->first;
    }

    optional<Key> askNxt(const Key& x) const {
        auto it = t.upper_bound({x, numeric_limits<int>::max()});
        return it == t.end() ? nullopt : optional<Key>(it->first);
    }

    optional<Key> askGE(const Key& x) const {
        auto it = t.lower_bound({x, 0});
        return it == t.end() ? nullopt : optional<Key>(it->first);
    }
};
