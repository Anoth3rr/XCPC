/*
用途：GNU PBDS gp_hash_table；开放寻址，常数小，适合大量唯一键的映射/集合。
限制：仅 GCC/libstdc++ 可用；增删可能使全部迭代器失效，不要长期保存迭代器。

HMap<K,V>：h[k]、insert、find、erase、size、empty、clear。
HSet<K>：insert、find、erase、size、empty、clear；不允许重复键。
默认 resize policy 不开放手动 resize/reserve，不能照搬 unordered_map::reserve。
Hash 对整数、字符串及 pair 均可用，并加入运行时盐以避免构造碰撞。
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
using HMap = __gnu_pbds::gp_hash_table<K, V, H, Eq>;

template <class K, class H = Hash, class Eq = equal_to<K>>
using HSet = __gnu_pbds::gp_hash_table<K, __gnu_pbds::null_type, H, Eq>;
