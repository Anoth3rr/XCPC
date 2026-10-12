vector<i64> a = {0, 7, 2, 5, 2, 9};
WaveletMatrix wm(a);

cout << wm.kth(2, 5, 2) << '\n';      // 2
cout << wm.kthLarge(1, 4, 2) << '\n'; // 5
