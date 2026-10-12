set<int> s = {5, 1, 5, 3}; // {1, 3, 5}

set<int, greater<int>> down = {5, 1, 5, 3}; // 降序容器：{5, 3, 1}

s.insert(x);
s.emplace(x);

s.clear();
s.empty();
s.size();

s.erase(x); // 删除等于 x 的元素
s.erase(it); // 删除迭代器 it 指向的元素
s.find(x); // 返回第一个等于 x 的迭代器，找不到时返回 end()
s.count(x); // 返回 x 的个数
s.lower_bound(x); // 默认升序：返回第一个 >= x 的迭代器
s.upper_bound(x); // 默认升序：返回第一个 > x 的迭代器
