/*
用途：GNU PBDS Patricia trie；维护唯一字符串，并直接枚举具有给定前缀的字符串。
限制：仅 GCC/libstdc++ 可用，节点开销较大；只需插入/查询时优先使用更轻的 08A。

insert/erase/find/size 与集合相同；prefix_range(s) 返回 [l,r)，其中均以 s 为前缀。
键不可重复；若需要出现次数，把次数另存在哈希表中或改用 08A。
*/
#ifdef int
#pragma push_macro("int")
#undef int
#define LYE_RESTORE_INT
#endif
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/trie_policy.hpp>
#ifdef LYE_RESTORE_INT
#pragma pop_macro("int")
#undef LYE_RESTORE_INT
#endif

using PTrie = __gnu_pbds::trie<
    string, __gnu_pbds::null_type,
    __gnu_pbds::trie_string_access_traits<>,
    __gnu_pbds::pat_trie_tag,
    __gnu_pbds::trie_prefix_search_node_update>;
