/*
用途：GNU PBDS 唯一键有序集合/映射，支持排名与第 k 小；10C 另行处理重复键。
限制：仅 GCC/libstdc++ 可用。默认红黑树；切换 Tag 可用 splay_tree_tag / ov_tree_tag。

OST<T> 常用操作（k 为 0-indexed）：
  insert/erase/find/lower_bound/upper_bound
  order_of_key(x)        严格小于 x 的元素数
  find_by_order(k)       第 k 小迭代器，越界等于 end()
  a.split(x, b)          a 保留 <= x，b 得到 > x
  a.join(b)              两棵树键域必须严格分离，完成后 b 为空
OMT<K,V> 还支持 t[k]、find_by_order(k)->second。
禁止用 less_equal 模拟 multiset：它不满足严格弱序，会破坏 erase/join 等接口。
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

template <class K, class Cmp = less<K>, class Tag = __gnu_pbds::rb_tree_tag>
using OST = __gnu_pbds::tree<K, __gnu_pbds::null_type, Cmp, Tag,
                            __gnu_pbds::tree_order_statistics_node_update>;

template <class K, class V, class Cmp = less<K>, class Tag = __gnu_pbds::rb_tree_tag>
using OMT = __gnu_pbds::tree<K, V, Cmp, Tag,
                            __gnu_pbds::tree_order_statistics_node_update>;
