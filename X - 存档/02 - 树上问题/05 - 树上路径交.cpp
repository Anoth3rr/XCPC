bool askOn(int a, int b, int x) {
    return askDis(a, x) + askDis(x, b) == askDis(a, b);
}

int ask(int a, int b, int c, int d) {
    vector<int> pnt = {a, b, c, d};
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            pnt.push_back(askLCA(pnt[i], pnt[j]));
        }
    }
    sort(pnt.begin(), pnt.end());
    pnt.erase(unique(pnt.begin(), pnt.end()), pnt.end());

    vector<int> g;
    for (int x : pnt) {
        if (askOn(a, b, x) && askOn(c, d, x)) g.push_back(x);
    }
    if (g.empty()) return 0;

    int ans = 1;
    for (int x : g) {
        for (int y : g) {
            ans = max(ans, askDis(x, y) + 1);
        }
    }
    return ans;
}
