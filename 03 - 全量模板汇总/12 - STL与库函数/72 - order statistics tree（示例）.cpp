#include <ext/pb_ds/tree_policy.hpp>
template<class T>
using ordered_set = __gnu_pbds::tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

ordered_set<int> os;
os.insert(5);
os.insert(1);
os.insert(7);

auto it = os.find_by_order(1); // 下标从 0 开始；越界时返回 end()
if (it != os.end()) cout << *it << '\n'; // 5

int rank = os.order_of_key(6); // 小于 6 的元素个数：2
