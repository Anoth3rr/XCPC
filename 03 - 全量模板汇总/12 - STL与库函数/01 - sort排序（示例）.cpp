vector<int> a = {4, 1, 5, 1, 3};
sort(a.begin(), a.end()); // 升序：{1, 1, 3, 4, 5}
sort(a.rbegin(), a.rend()); // 降序：{5, 4, 3, 1, 1}

stable_sort(a.begin(), a.end());
