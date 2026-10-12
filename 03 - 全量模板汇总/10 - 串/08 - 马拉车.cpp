string s;
cin >> s;
int n = s.length();
string t = "-#";
for (int i = 0; i < n; i++) {
    t += s[i];
    t += '#';
}
int m = t.length();
t += '+';
int mid = 0, r = 0;
vector<int> p(m);
for (int i = 1; i < m; i++) {
    p[i] = i < r ? min(p[2 * mid - i], r - i) : 1;
    while (t[i - p[i]] == t[i + p[i]]) p[i]++;
    if (i + p[i] > r) {
        r = i + p[i];
        mid = i;
    }
}
