int n;
cin >> n;
vector<int> a(n);
// iota(a.begin(), a.end(), 1LL);
for (auto &v : a) cin >> v;
sort(a.begin(), a.end());

do {
    for (auto v : a) {
        cout << v << " ";
    }
    cout << endl;
} while (next_permutation(a.begin(), a.end()));