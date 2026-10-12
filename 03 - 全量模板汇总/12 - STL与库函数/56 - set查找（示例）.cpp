set<int> s = {5, 1, 5, 3};
int x = 4;
auto it = s.lower_bound(x);
if (it != s.begin()) {
    auto pre = prev(it); // 最大的 < x 的元素：3
}

it = s.upper_bound(x);
if (it != s.end()) {
    auto nxt = it; // 最小的 > x 的元素：5
}

if (!s.empty()) {
    auto ed = *prev(s.end(), 1); // 返回最后一个元素：5
}
