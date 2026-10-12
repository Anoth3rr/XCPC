vector<int> a = {4, 1, 5, 1, 3};
int k = 2;
nth_element(a.begin(), a.begin() + k, a.end());
cout << a[k] << '\n'; // 找出第 k 小元素（k 从 0 开始）：3
