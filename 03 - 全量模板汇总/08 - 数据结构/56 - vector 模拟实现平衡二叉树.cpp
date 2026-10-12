#define ALL(x) x.begin(), x.end()
#define pre lower_bound
#define suf upper_bound
int n; cin >> n;
vector<int> ver;
for (int i = 1, op, x; i <= n; i++) {
    cin >> op >> x;
    if (op == 1) ver.insert(pre(ALL(ver), x), x);
    if (op == 2) ver.erase(pre(ALL(ver), x));
    if (op == 3) cout << pre(ALL(ver), x) - ver.begin() + 1 << endl;
    if (op == 4) cout << ver[x - 1] << endl;
    if (op == 5) cout << ver[pre(ALL(ver), x) - ver.begin() - 1] << endl;
    if (op == 6) cout << ver[suf(ALL(ver), x) - ver.begin()] << endl;
}
