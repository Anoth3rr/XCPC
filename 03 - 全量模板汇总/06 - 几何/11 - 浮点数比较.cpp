vector<pair<ld, int>> val = {
    {log(x) * pow(y, z), 0}, {log(x) * pow(z, y), 1}, {log(x) * y * z, 2},
    {log(x) * z * y, 3},     {log(y) * pow(x, z), 4}, {log(y) * pow(z, x), 5},
    {log(y) * x * z, 6},     {log(y) * z * x, 7},     {log(z) * pow(x, y), 8},
    {log(z) * pow(y, x), 9}, {log(z) * x * y, 10},    {log(z) * y * x, 11}};

sort(val.begin(), val.end(), [&](auto x, auto y) {
    if (equal(x.first, y.first)) return x.second < y.second; // queal比较两个浮点数是否相等
    return x.first > y.first;
});
cout << ans[val.front().second] << endl;
