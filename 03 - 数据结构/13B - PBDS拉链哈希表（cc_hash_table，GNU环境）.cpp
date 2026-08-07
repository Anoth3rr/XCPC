/*
用途：GNU PBDS cc_hash_table；拉链法哈希，适合希望避免开放寻址聚簇或存较大值的环境。
限制：仅 GCC/libstdc++ 可用；通常比 13A 多指针和节点分配开销，优先实测后选型。

HMap<K,V>：h[k]、insert、find、erase、size、empty、clear。
HSet<K>：insert、find、erase、size、empty、clear；不允许重复键。
13A/13B 是同一哈希字典问题的不同实现，通常只粘贴其中一个。
*/

using u64 = uint64_t;

#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#include <ext/pb_ds/assoc_container.hpp>
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

struct Hash {
    inline static const u64 r =
        chrono::steady_clock::now().time_since_epoch().count();

    static u64 mix(u64 x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    template <class T> size_t operator()(const T &x) const {
        return mix(u64(hash<T>{}(x)) + r);
    }

    template <class A, class B> size_t operator()(const pair<A, B> &x) const {
        u64 a = (*this)(x.first), b = (*this)(x.second);
        return mix(a ^ (b + 0x9e3779b97f4a7c15ULL + (a << 6) + (a >> 2)));
    }
};

template <class K, class V, class H = Hash, class Eq = equal_to<K>>
using HMap = __gnu_pbds::cc_hash_table<K, V, H, Eq>;

template <class K, class H = Hash, class Eq = equal_to<K>>
using HSet = __gnu_pbds::cc_hash_table<K, __gnu_pbds::null_type, H, Eq>;
