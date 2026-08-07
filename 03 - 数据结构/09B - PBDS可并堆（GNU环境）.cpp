/*
用途：GNU PBDS pairing heap；支持普通优先队列、修改/删除任意节点及破坏性合并。
限制：仅 GCC/libstdc++ 可用。默认 Cmp = less<T> 为大根堆，greater<T> 为小根堆。

push 返回的句柄可交给 modify/erase；join 后另一个堆清空。除 empty/size 外，操作空堆未定义。
pairing heap 的 push/join 为 O(1)，pop 为均摊 O(log n)，modify/erase 的实际复杂度依赖修改方向。
*/
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#include <ext/pb_ds/priority_queue.hpp>
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

template <class T, class Cmp = less<T>> struct Heap {
    using H = __gnu_pbds::priority_queue<T, Cmp, __gnu_pbds::pairing_heap_tag>;
    using It = typename H::point_iterator;
    H h;

    It push(const T &x) { return h.push(x); }
    void pop() { h.pop(); }
    const T &ask() const { return h.top(); }
    void modify(It p, const T &x) { h.modify(p, x); }
    void erase(It p) { h.erase(p); }
    void merge(Heap &o) { h.join(o.h); }
    bool askEmp() const { return h.empty(); }
    int askSz() const { return h.size(); }
    void clear() { h.clear(); }
};
