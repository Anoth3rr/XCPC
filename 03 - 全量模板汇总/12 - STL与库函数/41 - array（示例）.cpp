array<int, 4> a;
a.fill(3);
a[1] = 8;

sort(a.begin(), a.end());
cout << a.size() << ' ' << a.front() << ' ' << a.back() << '\n';
