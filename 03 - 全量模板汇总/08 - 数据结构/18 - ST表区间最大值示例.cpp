struct Op {
    int operator()(int x, int y) const { return max(x, y); }
};
vector<int> a = {{}, 9, 3, 1, 7, 5, 6, 0, 8};
ST<Op> st(a);
cout << mn.query(4, 8) << endl; // 8
