vector<i64> a = {{}, {2}, {3}, {16}, {9}};
SegTreeShrink t(a);

t.rangeSqrt(2, 4);                 // [2, 1, 4, 3]
cout << t.rangeSum(1, 4) << "\n";  // 10
t.rangeMod(1, 3, 3);               // [2, 1, 1, 3]
cout << t.rangeSum(1, 4) << "\n";  // 7
