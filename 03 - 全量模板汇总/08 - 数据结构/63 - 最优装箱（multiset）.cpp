void solve(){
    cin >> n >> c;
    for (int i = 1; i <= n; i++) cin >> a[i];
    multiset <int> s;
    for (int i = 1; i <= n; i++){
        auto it = s.lower_bound(a[i]);
        if (it == s.end()) s.insert(c - a[i]);
        else {
            int x = *it;
            // multiset 可以存放重复数据，如果是删除某个值的话，会去掉多个箱子
            // 导致答案错误，所以直接删除对应位置的元素
            s.erase(it);  
            s.insert(x - a[i]);
        }
    }
    cout << s.size() << "\n";
}
