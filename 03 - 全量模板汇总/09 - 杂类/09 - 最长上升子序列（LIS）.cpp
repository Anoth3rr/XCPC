// 严格递增子序列长度
template <class T> int LIS(const vector<T> &a) {
    vector<T> d;
    for (auto x : a) {
        auto it = lower_bound(d.begin(), d.end(), x);
        if (it == d.end()) d.push_back(x);
        else *it = x;
    }
    return d.size();
}

// 非降子序列长度
template <class T> int LNDS(const vector<T> &a) {
    vector<T> d;
    for (auto x : a) {
        auto it = upper_bound(d.begin(), d.end(), x);
        if (it == d.end()) d.push_back(x);
        else *it = x;
    }
    return d.size();
}

// 返回一组严格递增 LIS 的原数组下标（0-indexed）。
template <class T> vector<int> lis(const vector<T> &a) {
    int n = a.size();
    vector<T> d;
    vector<int> id, pre(n, -1);
    d.reserve(n);
    id.reserve(n);

    for (int i = 0; i < n; ++i) {
        int p = lower_bound(d.begin(), d.end(), a[i]) - d.begin();
        if (p > 0) pre[i] = id[p - 1];
        if (p == d.size()) {
            d.push_back(a[i]);
            id.push_back(i);
        } else {
            d[p] = a[i];
            id[p] = i;
        }
    }

    vector<int> ans;
    if (id.empty()) return ans;
    for (int cur = id.back(); cur != -1; cur = pre[cur]) {
        ans.push_back(cur);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

// 返回一组非降 LNDS 的原数组下标（0-indexed）。
template <class T> vector<int> lnds(const vector<T> &a) {
    int n = a.size();
    vector<T> d;
    vector<int> id, pre(n, -1);
    d.reserve(n);
    id.reserve(n);

    for (int i = 0; i < n; ++i) {
        int p = upper_bound(d.begin(), d.end(), a[i]) - d.begin();
        if (p > 0) pre[i] = id[p - 1];
        if (p == d.size()) {
            d.push_back(a[i]);
            id.push_back(i);
        } else {
            d[p] = a[i];
            id[p] = i;
        }
    }

    vector<int> ans;
    if (id.empty()) return ans;
    for (int cur = id.back(); cur != -1; cur = pre[cur]) {
        ans.push_back(cur);
    }
    reverse(ans.begin(), ans.end());
    return ans;
}
