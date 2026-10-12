i64 mask = (1LL << k) - 1;
i64 low = x & mask; // 取出后 k 位
i64 without_low = x & ~mask; // 后 k 位全置为 0
