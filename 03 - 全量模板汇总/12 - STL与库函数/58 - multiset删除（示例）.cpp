multiset<int> ms = {1, 2, 2, 3};
ms.erase(2); // 删除所有值为 2 的元素
auto it = ms.find(2);
if (it != ms.end()) ms.erase(it); // 只删除一个 2
