uint32_t x = 26; // 26=(11010)，固定为 32 位，不受 int 宏影响
cout << __builtin_popcount(x) << '\n'; // 返回含 1 的数量：3
cout << __builtin_ffs(x) << '\n'; // 最低位 1 的位置（右数，从 1 开始，x=0 返回 0）：2
cout << __builtin_ctz(x) << '\n'; // 末尾 0 的个数（x=0 时未定义）：1
cout << __builtin_clz(x) << '\n'; // 32 位 unsigned 的前导 0 个数（x=0 时未定义）：27

// c++17 替代写法
cout << (x == 0 ? 0 : 32 - __builtin_clz(x)) << '\n';

uint64_t y = 1ULL << 40;
cout << __builtin_popcountll(y) << '\n'; // 64 位 1 的数量：1
cout << (y == 0 ? 0 : 64 - __builtin_clzll(y)) << '\n'; // 位数：41
