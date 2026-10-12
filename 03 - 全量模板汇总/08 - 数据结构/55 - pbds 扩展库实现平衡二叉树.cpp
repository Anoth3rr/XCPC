#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using V = pair<int, int>;
tree<V, null_type, less<V>, rb_tree_tag, tree_order_statistics_node_update> ver;
map<int, int> dic;

int n; cin >> n;
for (int i = 1, op, x; i <= n; i++) {
    cin >> op >> x;
    if (op == 1) { // 插入一个元素x，允许重复
        ver.insert({x, ++dic[x]});
    } else if (op == 2) { // 删除元素x，若有重复，则任意删除一个
        ver.erase({x, dic[x]--});
    } else if (op == 3) { // 查询元素x的排名（排名定义为比当前数小的数的个数+1）
        cout << ver.order_of_key({x, 1}) + 1 << endl;
    } else if (op == 4) { // 查询排名为x的元素
        cout << ver.find_by_order(--x)->first << endl;
    } else if (op == 5) { // 查询元素x的前驱
        int idx = ver.order_of_key({x, 1}) - 1; // 无论x存不存在，idx都代表x的位置，需要-1
        cout << ver.find_by_order(idx)->first << endl;
    } else if (op == 6) { // 查询元素x的后继
        int idx = ver.order_of_key( {x, dic[x]}); // 如果x不存在，那么idx就是x的后继
        if (ver.find({x, 1}) != ver.end()) idx++; // 如果x存在，那么idx是x的位置，需要+1
        cout << ver.find_by_order(idx)->first << endl;
    }
}
