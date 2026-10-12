map<int, string> mp;
mp.insert({2, "two"});
mp.emplace(1, "one");
mp.try_emplace(3, "three"); // key 不存在时才构造 value
mp.insert_or_assign(2, "TWO"); // 存在则覆盖

mp[key] = value; // 随机访问，插入或覆盖
mp.find(x); // 返回第一个等于 x 的迭代器，找不到时返回 end()
mp.count(x); // 返回 x 的个数
mp.lower_bound(x); // 默认升序：返回第一个 >= x 的迭代器
mp.upper_bound(x); // 默认升序：返回第一个 > x 的迭代器
