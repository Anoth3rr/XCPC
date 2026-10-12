a.push_back(x);
a.emplace_back(x);
a.insert(a.end(), b.begin(), b.end()); // 在指定位置插入另一个容器的所有元素

a.pop_back(); // 不返回被删除的值
a.front();
a.back();
a.clear();
a.empty();
a.size();
