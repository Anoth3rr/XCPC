struct Info { i64 sum = 0; };
Info operator+(const Info &a, const Info &b) { return {a.sum + b.sum}; }

vector<Info> a = {{}, {1}, {2}, {3}, {4}, {5}};
SegTreeIter<Info> t(a);

cout << t.rangeQuery(2, 4).sum << "\n"; // 9
t.pointSet(3, {10});
cout << t.rangeQuery(2, 4).sum << "\n"; // 16
