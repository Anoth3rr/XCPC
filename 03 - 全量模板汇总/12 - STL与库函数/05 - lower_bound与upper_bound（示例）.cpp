vector<int> a = {1, 3, 3, 5};
int x = 3;
auto L_itr = lower_bound(a.begin(), a.end(), x); // 首个 >= x 的迭代器
auto R_itr = upper_bound(a.begin(), a.end(), x); // 首个 > x 的迭代器

auto L_idx = L_itr - a.begin(); // 下标：1
auto R_idx = R_itr - a.begin(); // 下标：3

auto count_x = R_itr - L_itr; // x 在 a 中出现的次数：2

if (L_itr != a.end()) {
    auto L_val = *L_itr; // 首个 >= x 的值：3
}
if (R_itr != a.end()) {
    auto R_val = *R_itr; // 首个 > x 的值：5
}
